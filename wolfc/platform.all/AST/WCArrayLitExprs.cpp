#include "WCArrayLitExprs.hpp"
#include "WCAssignExpr.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// ArrayLitExprs
//-----------------------------------------------------------------------------

bool ArrayLitExprs::peek(const Token * tokenPtr) {
    return AssignExpr::peek(tokenPtr);
}
    
ArrayLitExprs * ArrayLitExprs::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the first expression
    AssignExpr * expr = AssignExpr::parse(tokenPtr, alloc);
    WC_GUARD(expr, nullptr);
    
    // See if a comma follows (more args)
    if (tokenPtr->type == TokenType::kComma) {
        // Expression list with 2 or more expressions, consume the ',':
        ++tokenPtr;
        
        // There should be an expression list following:
        ArrayLitExprs * exprsList = ArrayLitExprs::parse(tokenPtr, alloc);
        WC_GUARD(exprsList, nullptr);
        
        // Return the node!
        return WC_NEW_AST_NODE(alloc, ArrayLitExprsMulti, *expr, *exprsList);
    }
    
    // Expression list with just 1 expression:
    return WC_NEW_AST_NODE(alloc, ArrayLitExprsSingle, *expr);
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
        return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kUnknown);
    }
    
    // Element types match so the element type of the array is this:
    return exprDataType;
}

WC_END_NAMESPACE