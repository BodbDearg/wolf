#include "WCModule.hpp"

#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDeclDef.hpp"
#include "WCFunc.hpp"
#include "WCParseCtx.hpp"
#include "WCVarDecl.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

Module::Module(llvm::LLVMContext & llvmCtx) :
    mLLVMCtx(llvmCtx),
    mEOFToken(nullptr)
{
    WC_EMPTY_FUNC_BODY();
}

Module::~Module() {
    // Defined here so callee doesn't need to know details of objects destroyed by std::unique_ptr...
    WC_EMPTY_FUNC_BODY();
}

const Token & Module::getStartToken() const {
    if (!mDeclDefs.empty()) {
        return mDeclDefs.front()->getStartToken();
    }
    
    WC_ASSERT(mEOFToken);
    return *mEOFToken;
}

const Token & Module::getEndToken() const {
    if (!mDeclDefs.empty()) {
        return mDeclDefs.back()->getEndToken();
    }
    
    WC_ASSERT(mEOFToken);
    return *mEOFToken;
}

bool Module::parse(ParseCtx & parseCtx) {
    while (DeclDef::peek(parseCtx.curTok)) {
        DeclDef * declDef = DeclDef::parse(parseCtx);
        WC_GUARD(declDef, false);
        declDef->mParent = this;
        mDeclDefs.push_back(declDef);
    }
    
    if (parseCtx.curTok->type != TokenType::kEOF) {
        parseError(parseCtx, "Expected EOF at end of module code!");
        mDeclDefs.clear();
        return false;
    }
    
    return true;
}

bool Module::generateCode() {
    // Clear out previous code and check we parsed ok
    mLLVMModule.reset();
    
    // TODO: alloc this memory with the linear allocator
    // Create module
    mLLVMModule.reset(new llvm::Module("WolfTest", mLLVMCtx));
    
    // The IR builder for the llvm compiler
    llvm::IRBuilder<> irBuilder(mLLVMCtx);
    
    // Declare C standard library functions we require
    declareCStdLibFuncsInModule();
    
    // Create the codegen context
    CodegenCtx codegenCtx(mLLVMCtx, irBuilder, *this);
    
    // Do the immediate forward code generation
    for (DeclDef * declDef : mDeclDefs) {
        WC_GUARD(declDef->codegen(codegenCtx), false);
    }
    
    // Now do any deferred code generation that needs to be done.
    // There may be multiple passes here for this loop:
    while (!codegenCtx.deferredCodegenCallbacks.empty()) {
        // Make a copy of the list because deferred codegen can itself cause more deferred codegen.
        // Also clear the list so the callbacks can start generating their own deferred logic:
        auto deferredCodegenCallbacks = codegenCtx.deferredCodegenCallbacks;
        codegenCtx.deferredCodegenCallbacks.clear();
        
        // Do deferred codegen:
        for (const auto & callback : deferredCodegenCallbacks) {
            WC_GUARD(callback(codegenCtx), false);
        }
    }

    return true;    // Suceeded!
}

bool Module::wasCodeGeneratedOk() {
    // FIXME: this can be not null and still not be generated ok
    return mLLVMModule.get() != nullptr;
}

void Module::dumpIRCodeToStdout() {
    WC_GUARD_ASSERT(mLLVMModule.get());
    mLLVMModule->dump();
}

bool Module::registerFunc(Func & func) {
    std::string funcName = func.name();
    
    if (getFunc(funcName)) {
        return false;
    }
    
    mFuncs[funcName] = &func;
    return true;
}

Func * Module::getFunc(const std::string & name) const {
    auto iter = mFuncs.find(name);
    WC_GUARD(iter != mFuncs.end(), nullptr);
    return iter->second;
}

DataValue * Module::createVar(const char * varName,
                              DataType & dataType,
                              llvm::Constant * initializer,
                              CodegenCtx & cgCtx,
                              VarDecl & callingNode)
{
    // If the variable already exists in this scope then creation fails:
    {
        auto iter = mVarValues.find(varName);
        
        if (iter != mVarValues.end()) {
            return nullptr;
        }
    }

    // Compile the variable llvm type:
    WC_GUARD(dataType.codegenLLVMTypeIfRequired(cgCtx, callingNode), nullptr);
    
    // The variable must have an llvm type:
    if (!dataType.mLLVMType) {
        compileError("Variable '%s' of type '%s' is not an llvm primitive type! Cannot create a global variable to hold it!",
                     varName,
                     dataType.name().c_str());
        
        return nullptr;
    }
    
    // TODO: don't use new here, use the linear allocator
    // Make the data value:
    DataValue & dataValue = mVarValues[varName];
    
    dataValue.declaringNode = &callingNode;
    dataValue.requiresLoad = true;
    dataValue.type = &dataType;
    dataValue.value = new llvm::GlobalVariable(*mLLVMModule.get(),
                                               dataType.mLLVMType,
                                               false,                               // Not constant
                                               llvm::GlobalValue::PrivateLinkage,
                                               initializer,
                                               varName);
    
    return &dataValue;
}

DataValue * Module::getVar(const char * varName) const {
    auto iter = mVarValues.find(varName);
    
    if (iter != mVarValues.end()) {
        return const_cast<DataValue*>(&iter->second);
    }
    
    return nullptr;     // Variable not found!
}

void Module::declareCStdLibFuncsInModule() {
    // Declare the c standard library function 'printf'
    llvm::FunctionType * printfFnType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(mLLVMCtx),
        llvm::ArrayRef<llvm::Type*>{
            llvm::Type::getInt8Ty(mLLVMCtx)->getPointerTo()
        },
        true);

    mLLVMModule->getOrInsertFunction("printf", printfFnType);

    // Declare the c standard library function 'scanf'
    llvm::FunctionType * scanfFnType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(mLLVMCtx),
        llvm::ArrayRef<llvm::Type*>{
            llvm::Type::getInt8Ty(mLLVMCtx)->getPointerTo()
        },
        true);

    mLLVMModule->getOrInsertFunction("scanf", scanfFnType);

    // Declare the c standard library function 'getchar'
    llvm::FunctionType * getcharFnType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(mLLVMCtx),
        llvm::ArrayRef<llvm::Type*>{},
        false);

    mLLVMModule->getOrInsertFunction("getchar", getcharFnType);

    // Declare the c standard library function 'time'
    llvm::FunctionType * timeFnType = llvm::FunctionType::get(
        llvm::Type::getInt64Ty(mLLVMCtx),
        llvm::ArrayRef<llvm::Type*>{
            llvm::Type::getInt64Ty(mLLVMCtx)->getPointerTo(),
        },
        false);

    mLLVMModule->getOrInsertFunction("time", timeFnType);

    // Declare the c standard library function 'rand'
    llvm::FunctionType * randFnType = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(mLLVMCtx),
        llvm::ArrayRef<llvm::Type*>{},
        false);

    mLLVMModule->getOrInsertFunction("rand", randFnType);

    // Declare the c standard library function 'srand'
    llvm::FunctionType * srandFnType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(mLLVMCtx),
        llvm::ArrayRef<llvm::Type*>{
            llvm::Type::getInt32Ty(mLLVMCtx),
        },
        false);
    
    mLLVMModule->getOrInsertFunction("srand", srandFnType);

    // Declare the c standard library function 'abort'
    llvm::FunctionType * abortFnType = llvm::FunctionType::get(
        llvm::Type::getVoidTy(mLLVMCtx),
        llvm::ArrayRef<llvm::Type*>{},
        false);

    mLLVMModule->getOrInsertFunction("abort", abortFnType);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
