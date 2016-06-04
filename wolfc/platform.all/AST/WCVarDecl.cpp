#include "WCVarDecl.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCIdentifier.hpp"
#include "WCPrimitiveDataTypes.hpp"
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
        parseError(*tokenPtr, "Expected keyword 'var' at start of variable declaration!");
        return nullptr;
    }
    
    const Token * varToken = tokenPtr;
    ++tokenPtr;     // Skip 'var'
    
    // Parse the identifier ahead
    Identifier * ident = Identifier::parse(tokenPtr);
    WC_GUARD(ident, nullptr);
    
    // Parse the '='
    if (tokenPtr->type != TokenType::kEquals) {
        parseError(*tokenPtr, "Expected '=' following variable name for variable declaration!");
        return nullptr;
    }
    
    ++tokenPtr;
    
    // Parse the assign expression and return result of parsing
    AssignExpr * expr = AssignExpr::parse(tokenPtr);
    WC_GUARD(expr, nullptr);
    return new VarDecl(*varToken, *ident, *expr);
}

VarDecl::VarDecl(const Token & token, Identifier & ident, AssignExpr & expr) :
    mStartToken(token),
    mIdent(ident),
    mExpr(expr)
{
    mIdent.mParent = this;
    mExpr.mParent = this;
}

const Token & VarDecl::getStartToken() const {
    return mStartToken;
}

const Token & VarDecl::getEndToken() const {
    return mExpr.getEndToken();
}

llvm::Value * VarDecl::generateCode(const CodegenCtx & cgCtx) {
    // TODO: allow more than int
    const DataType & exprType = mExpr.getDataType();
    
    if (!exprType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Var declaration must be assigned type 'int' not '%s'!", exprType.name());
        return nullptr;
    }
    
    // Grab the parent scope
    Scope * parentScope = getParentScope();
    
    if (!parentScope) {
        compileError("Can't codegen, no parent scope!");
        return nullptr;
    }
    
    // Alloc room for the variable on left
    llvm::Value * leftValue = parentScope->getVariable(mIdent.mToken.data.strVal.ptr);
    
    if (leftValue) {
        compileError("The variable '%s' has been redefined!", mIdent.mToken.data.strVal.ptr);
        return nullptr;
    }
        
    leftValue = parentScope->getOrCreateVariable(mIdent.mToken.data.strVal.ptr, cgCtx);
    
    if (!leftValue) {
        compileError("Can't codegen, unable to allocate room for variable!");
        return nullptr;
    }
    
    // Now evaluate the right:
    llvm::Value * rightValue = mExpr.generateCode(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate store instruction:
    return cgCtx.irBuilder.CreateStore(rightValue, leftValue);
}

WC_END_NAMESPACE
