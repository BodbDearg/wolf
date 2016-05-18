#include "WCVarDecl.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIdentifier.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool VarDecl::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kVar;
}

VarDecl * VarDecl::parse(const Token *& tokenPtr) {
    // Parse 'var' keyword
    if (tokenPtr->type != TokenType::kVar) {
        // TODO: add 'let' to docs here?
        error(*tokenPtr, "Expected keyword 'var' at start of variable declaration!");
        return nullptr;
    }
    
    ++tokenPtr;     // Skip 'var'
    
    // Parse the identifier ahead
    Identifier * ident = Identifier::parse(tokenPtr);
    WC_GUARD(ident, nullptr);
    
    // Parse the '='
    if (tokenPtr->type != TokenType::kEquals) {
        error(*tokenPtr, "Expected '=' following variable name for variable declaration!");
        return nullptr;
    }
    
    ++tokenPtr;
    
    // Parse the assign expression and return result of parsing
    AssignExpr * expr = AssignExpr::parse(tokenPtr);
    WC_GUARD(expr, nullptr);
    return new VarDecl(*ident, *expr);
}

VarDecl::VarDecl(Identifier & ident, AssignExpr & expr) : mIdent(ident), mExpr(expr) {
    mIdent.mParent = this;
    mExpr.mParent = this;
}

llvm::Value * VarDecl::generateCode(const CodegenCtx & cgCtx) {
    // Grab the parent scope
    Scope * parentScope = getParentScope();
    
    if (!parentScope) {
        error("Can't codegen, no parent scope!");
        return nullptr;
    }
    
    // Alloc room for the variable on left
    llvm::Value * leftValue = parentScope->getVariable(mIdent.mToken.data.strVal.ptr);
    
    if (leftValue) {
        error("The variable '%s' has been redefined!", std::unique_ptr<char[]>(mIdent.getUtf8Name()).get());
        return nullptr;
    }
        
    leftValue = parentScope->getOrCreateVariable(mIdent.mToken.data.strVal.ptr, cgCtx);
    
    if (!leftValue) {
        error("Can't codegen, unable to allocate room for variable!");
        return nullptr;
    }
    
    // Now evaluate the right:
    llvm::Value * rightValue = mExpr.generateCode(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate store instruction:
    return cgCtx.irBuilder.CreateStore(rightValue, leftValue);
}

WC_END_NAMESPACE
