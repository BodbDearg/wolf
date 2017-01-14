#include "WCArrayLitExprs.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCDataTypeId.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssignExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// ArrayLitExprs
//-----------------------------------------------------------------------------
bool ArrayLitExprs::peek(const Token * tokenPtr) {
    return AssignExpr::peek(tokenPtr);
}
    
ArrayLitExprs * ArrayLitExprs::parse(ParseCtx & parseCtx) {
    // Parse the first expression
    AssignExpr * expr = AssignExpr::parse(parseCtx);
    WC_GUARD(expr, nullptr);
    
    // See if a comma follows (more args)
    if (parseCtx.curTok->type == TokenType::kComma) {
        // Expression list with 2 or more expressions, consume the ',':
        parseCtx.nextTok();
        
        // There should be an expression list following:
        ArrayLitExprs * exprsList = ArrayLitExprs::parse(parseCtx);
        WC_GUARD(exprsList, nullptr);
        
        // Return the node!
        return WC_NEW_AST_NODE(parseCtx, ArrayLitExprsMulti, *expr, *exprsList);
    }
    
    // Expression list with just 1 expression:
    return WC_NEW_AST_NODE(parseCtx, ArrayLitExprsSingle, *expr);
}

//-----------------------------------------------------------------------------
// ArrayLitExprsSingle
//-----------------------------------------------------------------------------
ArrayLitExprsSingle::ArrayLitExprsSingle(AssignExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & ArrayLitExprsSingle::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & ArrayLitExprsSingle::getEndToken() const {
    return mExpr.getEndToken();
}

bool ArrayLitExprsSingle::isConstExpr() const {
    return mExpr.isConstExpr();
}

size_t ArrayLitExprsSingle::numExprs() const {
    return 1;
}

void ArrayLitExprsSingle::getExprs(std::vector<AssignExpr*> & exprs) const {
    exprs.push_back(&mExpr);
}

DataType & ArrayLitExprsSingle::getElementType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// ArrayLitExprsMulti
//-----------------------------------------------------------------------------
ArrayLitExprsMulti::ArrayLitExprsMulti(AssignExpr & expr, ArrayLitExprs & exprsList) :
    mExpr(expr),
    mExprsList(exprsList)
{
    mExpr.mParent = this;
    mExprsList.mParent = this;
}

const Token & ArrayLitExprsMulti::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & ArrayLitExprsMulti::getEndToken() const {
    return mExprsList.getEndToken();
}

bool ArrayLitExprsMulti::isConstExpr() const {
    return mExpr.isConstExpr() && mExprsList.isConstExpr();
}

size_t ArrayLitExprsMulti::numExprs() const {
    return 1 + mExprsList.numExprs();
}

void ArrayLitExprsMulti::getExprs(std::vector<AssignExpr*> & exprs) const {
    exprs.push_back(&mExpr);
    mExprsList.getExprs(exprs);
}

DataType & ArrayLitExprsMulti::getElementType() const {
    // Get the data type of the expression
    DataType & exprDataType = mExpr.dataType();
    
    // Now get the type of the other expressions and see if they match.
    // If we can't match types the overall element type of the array is ambiguous/unknown.
    DataType & exprsListDataType = mExprsList.getElementType();
    
    if (!exprDataType.equals(exprsListDataType)) {
        // Can't determine element type, ambiguous!
        return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kUnknown);
    }
    
    // Element types match so the element type of the array is this:
    return exprDataType;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
