#include "WCOpStmnt.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCTernaryExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// OpStmnt
//-----------------------------------------------------------------------------

bool OpStmnt::peek(const Token * tokenPtr) {
    return TernaryExpr::peek(tokenPtr);
}

OpStmnt * OpStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the left expression
    TernaryExpr * leftExpr = TernaryExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a '=' for assignment
    if (tokenPtr->type == TokenType::kEquals) {
        // Assign statement with assign. Skip the '='
        ++tokenPtr;
        
        // Parse the following expression and create the AST node
        TernaryExpr * rightExpr = TernaryExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, OpStmntAssign, *leftExpr, *rightExpr);
    }

    // Op statement that is just a basic expression
    return WC_NEW_AST_NODE(alloc, OpStmntExpr, *leftExpr);
}

//-----------------------------------------------------------------------------
// OpStmntExpr
//-----------------------------------------------------------------------------

OpStmntExpr::OpStmntExpr(TernaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & OpStmntExpr::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & OpStmntExpr::getEndToken() const {
    return mExpr.getEndToken();
}

bool OpStmntExpr::codegen(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx) != nullptr;
}

//-----------------------------------------------------------------------------
// OpStmntAssign
//-----------------------------------------------------------------------------

OpStmntAssign::OpStmntAssign(TernaryExpr & leftExpr, TernaryExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & OpStmntAssign::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & OpStmntAssign::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool OpStmntAssign::codegen(CodegenCtx & cgCtx) {
    // Left side of expression must be an lvalue!
    if (!mLeftExpr.isLValue()) {
        compileError("Can't assign to an rvalue! Must assign to a valid variable!");
        return false;
    }
    
    // TODO: support auto type promotion and the lark
    // The data type of the right must match the left
    const DataType & leftDataType = mLeftExpr.dataType();
    const DataType & rightDataType = mRightExpr.dataType();
    
    // TODO: print variable name here
    if (!leftDataType.equals(rightDataType)) {
        compileError("Can't assign expression of type '%s' to variable of type '%s'!",
                     rightDataType.name().c_str(),
                     leftDataType.name().c_str());
        
        return false;
    }
    
    // Evaluate left side
    llvm::Value * leftValue = mLeftExpr.codegenAddrOf(cgCtx);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, nullptr);
    
    // Generate store instruction:
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(rightValue, leftValue);
    WC_ASSERT(storeInst);
    return true;
}

WC_END_NAMESPACE
