#include "Func.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "FuncArg.hpp"
#include "Identifier.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"
#include "Type.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <set>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool Func::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kFunc;
}

Func * Func::parse(ParseCtx & parseCtx) {
    // Must be a function ahead:
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected function!");
        return nullptr;
    }
    
    // Skip the 'func' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Parse the function identifier:
    Identifier * identifier = Identifier::parse(parseCtx);
    WC_GUARD(identifier, nullptr);
    
    // Expect '('
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("'(' expected following function name!");
        return nullptr;
    }
    
    parseCtx.nextTok();     // Skip '('
    
    // Parse any function arguments that follow:
    std::vector<FuncArg*> funcArgs;
    
    while (FuncArg::peek(parseCtx.tok())) {
        // Parse the arg
        FuncArg * funcArg = FuncArg::parse(parseCtx);
        
        if (funcArg) {
            funcArgs.push_back(funcArg);
        }
        
        // See if a ',' follows
        if (parseCtx.tok()->type == TokenType::kComma) {
            // Skip the ','
            parseCtx.nextTok();
            
            // Expect an argument following
            if (!FuncArg::peek(parseCtx.tok())) {
                parseCtx.error("Expect an argument following ','!");
            }
        }
    }
    
    // Expect ')'
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("')' expected to close args list of function!");
        return nullptr;
    }
    
    parseCtx.nextTok();     // Skip ')'
    
    // See if a '->' follows for function explicit return type.
    // If it is not present then a 'void' return type is assumed.
    Type * returnType = nullptr;
    
    if (parseCtx.tok()->type == TokenType::kOpArrow) {
        // Explicit return type, skip the '->' first
        parseCtx.nextTok();
        
        // Now parse the return type, if that fails then bail
        returnType = Type::parse(parseCtx);
        WC_GUARD(returnType, nullptr);
    }
    
    // Parse the inner function scope:
    Scope * scope = Scope::parse(parseCtx);
    WC_GUARD(scope, nullptr);
    
    // Must be terminated by an 'end' token
    if (parseCtx.tok()->type != TokenType::kEnd) {
        parseCtx.error("'end' expected to terminate function definition!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Done: return the parsed function
    return WC_NEW_AST_NODE(parseCtx,
                           Func,
                           *startToken,
                           *identifier,
                           std::move(funcArgs),
                           returnType,
                           *scope,
                           *endToken);
}

Func::Func(const Token & startToken,
           Identifier & identifier,
           std::vector<FuncArg*> && funcArgs,
           Type * explicitReturnType,
           Scope & scope,
           const Token & endToken)
:
    mStartToken(startToken),
    mIdentifier(identifier),
    mFuncArgs(funcArgs),
    mExplicitReturnType(explicitReturnType),
    mScope(scope),
    mEndToken(endToken)
{
    mIdentifier.mParent = this;
    
    for (FuncArg * funcArg : mFuncArgs) {
        funcArg->mParent = this;
    }
    
    if (mExplicitReturnType) {
        mExplicitReturnType->mParent = this;
    }
    
    mScope.mParent = this;
}

void Func::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & Func::getStartToken() const {
    return mStartToken;
}

const Token & Func::getEndToken() const {
    return mEndToken;
}

const char * Func::name() const {
    return mIdentifier.name();
}

#warning FIXME - Codegen
#if 0
bool Func::codegen(CodegenCtx & cgCtx) {
    // TODO: verify all codepaths return a value if there is a return value
    
    // Register the function with the parent module.
    Module * module = firstParentOfType<Module>();
    WC_ASSERT(module);
    
    if (!module->registerFunc(*this)) {
        compileError("Duplicate function named '%s' defined!", name());
        return false;
    }
    
    // Get the list of function args
    std::vector<FuncArg*> funcArgs;
    
    if (mArgList) {
        funcArgs.reserve(mArgList->numArgs());
        mArgList->getArgs(funcArgs);
    }
    
    // Check for duplicate argument names
    WC_GUARD(compileCheckForDuplicateArgNames(funcArgs), false);
    
    // Generate the code for each arg (llvm types):
    for (FuncArg * arg : funcArgs) {
        WC_GUARD(arg->codegen(cgCtx, *this), false);
    }
    
    // Get the llvm types for the function arguments
    std::vector<llvm::Type*> fnArgTypesLLVM;
    WC_GUARD(getLLVMArgTypes(funcArgs, fnArgTypesLLVM), false);
    
    // Generate the llvm type for the function return:
    llvm::Type * fnRetType = codegenLLVMReturnType(cgCtx);
    WC_GUARD(fnRetType, false);
    
    // Create the function signature:
    // TODO: support varargs
    llvm::FunctionType * fnType = llvm::FunctionType::get(fnRetType,
                                                          fnArgTypesLLVM,
                                                          false);
    WC_ASSERT(fnType);
    
    // Create the function object itself
    // TODO: support different linkage types
    mLLVMFunc = llvm::Function::Create(fnType,
                                       llvm::Function::ExternalLinkage,
                                       mIdentifier.name(),
                                       cgCtx.module.getLLVMModule());
    WC_ASSERT(mLLVMFunc);
    
    // Save a list of the function arguments for later lookup by variables
    auto & llvmArgList = mLLVMFunc->getArgumentList();
    
    for (auto & llvmArg : llvmArgList) {
        size_t argNum = llvmArg.getArgNo();
        WC_ASSERT(argNum < funcArgs.size());
        FuncArg * funcArg = funcArgs[argNum];
        WC_ASSERT(funcArg);
        DataType & argDataType = funcArg->dataType();
        bool requiresLoad = argDataType.requiresStorage();
        
        mArgValues[funcArg->name()] = DataValue(funcArg,
                                                &llvmArg,
                                                &argDataType,
                                                requiresLoad);
    }
    
    // Request deferred codegen for the function body. Will generate the body code for all functions
    // after the function definitions themselves have been parsed...
    cgCtx.deferredCodegenCallbacks.push_back([=](CodegenCtx & deferredCgCtx){
        return deferredCodegen(deferredCgCtx);
    });
    
    return true;    // All good!
}

bool Func::getLLVMArgTypes(const std::vector<FuncArg*> & funcArgs,
                           std::vector<llvm::Type*> & outputArgTypes) const
{
    // Process all of the argument data types
    for (FuncArg * arg : funcArgs) {
        WC_ASSERT(arg);
        DataType & argDataType = arg->dataType();
        
        // Only valid, sized args type are allowed:
        if (argDataType.isSized()) {
            // Make sure it has a valid llvm type
            llvm::Type * llvmType = argDataType.mLLVMType;
            
            if (!llvmType) {
                compileError("Failed to determine the llvm type for argument '%s' of type '%s'!",
                             arg->name(),
                             arg->dataType().name().c_str());
                
                return false;
            }

            // If the data type requires storage then we must make a copy of it on the 
            // stack and pass it by pointer.
            if (argDataType.requiresStorage()) {
                llvm::Type * ptrToLLVMType = llvmType->getPointerTo();
                WC_ASSERT(ptrToLLVMType);
                outputArgTypes.push_back(ptrToLLVMType);
            }
            else {
                outputArgTypes.push_back(llvmType);
            }
        }
        else {
            // Type is not sized (e.g 'void')
            compileError("Invalid data type specified for argument '%s' of type '%s'!",
                         arg->name(),
                         arg->dataType().name().c_str());
            
            return false;
        }
    }
    
    return true;    // All good!
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
