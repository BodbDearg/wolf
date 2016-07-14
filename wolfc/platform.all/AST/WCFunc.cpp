#include "WCFunc.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIDeferredCodegenStmnt.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool Func::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kFunc;
}

Func * Func::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Must be a function ahead:
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected function!");
        return nullptr;
    }
    
    // Skip the 'func' token and save location
    const Token * startToken = tokenPtr;
    ++tokenPtr;
    
    // Parse the function identifier:
    Identifier * identifier = Identifier::parse(tokenPtr, alloc);
    WC_GUARD(identifier, nullptr);
    
    // Expect '('
    if (tokenPtr->type != TokenType::kLParen) {
        parseError(*tokenPtr, "'(' expected following function name!");
        return nullptr;
    }
    
    ++tokenPtr; // Skip '('
    
    // Expect ')'
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "')' expected to close args list of function!");
        return nullptr;
    }
    
    ++tokenPtr; // Skip ')'
    
    // Parse the inner function scope:
    Scope * scope = Scope::parse(tokenPtr, alloc);
    WC_GUARD(scope, nullptr);
    
    // Must be terminated by an 'end' token
    if (tokenPtr->type != TokenType::kEnd) {
        parseError(*tokenPtr, "'end' expected to terminate function definition!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = tokenPtr;
    ++tokenPtr;
    
    // Done: return the parsed function
    return WC_NEW_AST_NODE(alloc, Func, *startToken, *identifier, *scope, *endToken);
}

Func::Func(const Token & startToken,
           Identifier & identifier,
           Scope & scope,
           const Token & endToken)
:
    mStartToken(startToken),
    mIdentifier(identifier),
    mScope(scope),
    mEndToken(endToken)
{
    mIdentifier.mParent = this;
    mScope.mParent = this;
}

const Token & Func::getStartToken() const {
    return mStartToken;
}

const Token & Func::getEndToken() const {
    return mEndToken;
}

bool Func::codegen(CodegenCtx & cgCtx) {
    // TODO: check for duplicate function definitions
    
    // Create the function signature:
    // TODO: support return types and arguments
    // TODO: support varargs
    llvm::FunctionType * fnType = llvm::FunctionType::get(llvm::Type::getVoidTy(cgCtx.llvmCtx), {}, false);
    
    // Create the function object itself
    // TODO: support different linkage types
    llvm::Function * fn = llvm::Function::Create(fnType,
                                                 llvm::Function::ExternalLinkage,
                                                 mIdentifier.name(),
                                                 &cgCtx.module);
    
    // Create the function entry block and set it as the insert point for ir builder
    llvm::BasicBlock * fnEntryBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "func_entry_block", fn);
    cgCtx.irBuilder.SetInsertPoint(fnEntryBlock);
    
    // Do basic forward code generation (most code is generated this way):
    if (!mScope.codegenStmnt(cgCtx)) {
        return false;
    }
    
    // Do deferred code generation for 'next', 'break' and 'return' statements.
    // Note: need to push and pop current insert block because this process may modify insert block:
    cgCtx.pushInsertBlock();
    
    while (!cgCtx.deferredCodegenStmnts.empty()) {
        IDeferredCodegenStmnt * stmnt = cgCtx.deferredCodegenStmnts.back();
        cgCtx.deferredCodegenStmnts.pop_back();
        
        if (!stmnt->deferredCodegenStmnt(cgCtx)) {
            return false;
        }
    }
    
    cgCtx.popInsertBlock();
    
    // Create the return from the function
    // TODO: Support returning values
    cgCtx.irBuilder.CreateRetVoid();
    return true;
}

WC_END_NAMESPACE
