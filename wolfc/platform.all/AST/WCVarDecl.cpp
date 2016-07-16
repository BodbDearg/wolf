#include "WCVarDecl.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool VarDecl::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kVar;
}

VarDecl * VarDecl::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse 'var' keyword
    if (tokenPtr->type != TokenType::kVar) {
        // TODO: add 'let' to docs here?
        parseError(*tokenPtr, "Expected keyword 'var' at start of variable declaration!");
        return nullptr;
    }
    
    const Token * varToken = tokenPtr;
    ++tokenPtr;     // Skip 'var'
    
    // Parse the identifier ahead
    Identifier * ident = Identifier::parse(tokenPtr, alloc);
    WC_GUARD(ident, nullptr);
    
    // Parse the '='
    if (tokenPtr->type != TokenType::kEquals) {
        parseError(*tokenPtr, "Expected '=' following variable name for variable declaration!");
        return nullptr;
    }
    
    ++tokenPtr;
    
    // Parse the assign expression and return result of parsing
    AssignExpr * expr = AssignExpr::parse(tokenPtr, alloc);
    WC_GUARD(expr, nullptr);
    return WC_NEW_AST_NODE(alloc, VarDecl, *varToken, *ident, *expr);
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

bool VarDecl::codegen(CodegenCtx & cgCtx) {
    // Grab the parent scope
    Scope * parentScope = getParentScope();
    
    if (!parentScope) {
        compileError("Can't codegen, no parent scope!");
        return false;
    }
    
    // Create the variable. If this fails then the variable already exists:
    const DataType & exprType = mExpr.dataType();
    const DataValue * leftValue = parentScope->createVariable(mIdent.mToken.data.strVal.ptr, exprType, cgCtx);
    
    if (!leftValue) {
        compileError("The variable '%s' has been redefined!", mIdent.mToken.data.strVal.ptr);
        return false;
    }
    
    // Now evaluate the right:
    llvm::Value * rightValue = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, false);
    
    // TODO: this won't work for non primitive types
    // Generate store instruction:
    return cgCtx.irBuilder.CreateStore(rightValue, leftValue->value) != nullptr;
}

WC_END_NAMESPACE
