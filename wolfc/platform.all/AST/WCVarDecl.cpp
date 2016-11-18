#include "WCVarDecl.hpp"

#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
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
    // See if this is a local or global var. If it is a local var then there will be a parent scope:
    Scope * parentScope = getParentScope();
    
    if (parentScope) {
        return codegenAsLocalVar(cgCtx, *parentScope);     // Local variable!
    }
    
    return codegenAsGlobalVar(cgCtx);
}

bool VarDecl::codegenAsLocalVar(CodegenCtx & cgCtx, Scope & parentScope) {
    // Create the variable. If this fails then the variable already exists:
    DataType & exprType = mExpr.dataType();
    DataValue * leftValue = parentScope.createVar(mIdent.mToken.data.strVal.ptr,
                                                  exprType,
                                                  cgCtx,
                                                  *this);
    
    if (!leftValue) {
        compileError("The local variable '%s' has been redefined!", mIdent.mToken.data.strVal.ptr);
        return false;
    }
    
    // Now evaluate the right:
    llvm::Value * rightValue = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, false);

    // Generate store instruction:
    return cgCtx.irBuilder.CreateStore(rightValue, leftValue->value) != nullptr;
}

bool VarDecl::codegenAsGlobalVar(CodegenCtx & cgCtx) {
    // Now evaluate the right expression:
    llvm::Constant * rightValue = mExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightValue, false);
    
    // Create the variable. If this fails then the variable already exists:
    DataType & exprType = mExpr.dataType();
    DataValue * leftValue = cgCtx.module.createVar(mIdent.mToken.data.strVal.ptr,
                                                   exprType,
                                                   rightValue,
                                                   cgCtx,
                                                   *this);
    
    if (!leftValue) {
        compileError("The global variable '%s' has been redefined!", mIdent.mToken.data.strVal.ptr);
        return false;
    }
    
    return true;    // All good!
}

WC_END_NAMESPACE
