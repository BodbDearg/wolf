#include "WCModule.hpp"

#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"
#include "WCDeclDef.hpp"
#include "WCFunc.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCVarDecl.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

Module * Module::parse(ParseCtx & parseCtx) {
    // Parse a list of decldefs for the module
    std::vector<DeclDef*> declDefs;
    
    while (DeclDef::peek(parseCtx.curTok)) {
        DeclDef * declDef = DeclDef::parse(parseCtx);
        WC_GUARD(declDef, nullptr);
        declDefs.push_back(declDef);
    }
    
    // Expect to be at EOF at this point
    if (parseCtx.curTok->type != TokenType::kEOF) {
        parseCtx.error("Expected EOF at end of module code!");
        return nullptr;
    }
    
    // Okay, create and return the module
    return WC_NEW_AST_NODE(parseCtx, Module, std::move(declDefs), *parseCtx.curTok);
}

Module::Module(std::vector<DeclDef*> && declDefs, const Token & eofToken) :
    mDeclDefs(declDefs),
    mEOFToken(eofToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & Module::getStartToken() const {
    if (!mDeclDefs.empty()) {
        return mDeclDefs.front()->getStartToken();
    }
    
    return mEOFToken;
}

const Token & Module::getEndToken() const {
    return mEOFToken;
}

#warning FIXME - Codegen
#if 0
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
#endif

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

#warning FIXME - Codegen
#if 0
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
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
