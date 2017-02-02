#include "NotExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "CmpExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// NotExpr
//-----------------------------------------------------------------------------
bool NotExpr::peek(const Token * tokenPtr) {
    return  tokenPtr->type == TokenType::kNot ||
            tokenPtr->type == TokenType::kTilde ||
            CmpExpr::peek(tokenPtr);
}

NotExpr * NotExpr::parse(ParseCtx & parseCtx) {
    // Save the first token:
    const Token * startToken = parseCtx.tok();
    
    // See if an actual 'not' or bitwise not ('~') expression:
    if (startToken->type == TokenType::kNot) {
        // Logical 'not': skip 'not'
        parseCtx.nextTok();
        
        // Parse the expression following
        NotExpr * notExpr = NotExpr::parse(parseCtx);
        WC_GUARD(notExpr, nullptr);
        
        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(parseCtx, NotExprLNot, *notExpr, *startToken);
    }
    else if (startToken->type == TokenType::kTilde) {
        // Bitwise 'not': skip 'not'
        parseCtx.nextTok();

        // Parse the expression following
        NotExpr * notExpr = NotExpr::parse(parseCtx);
        WC_GUARD(notExpr, nullptr);

        // Alright, return the parsed expr
        return WC_NEW_AST_NODE(parseCtx, NotExprBNot, *notExpr, *startToken);
    }
    
    // No 'not'. Just parse an ordinary no-op expression
    CmpExpr * addSubExpr = CmpExpr::parse(parseCtx);
    WC_GUARD(addSubExpr, nullptr);
    return WC_NEW_AST_NODE(parseCtx, NotExprNoOp, *addSubExpr);
}

//-----------------------------------------------------------------------------
// NotExprNoOp
//-----------------------------------------------------------------------------
NotExprNoOp::NotExprNoOp(CmpExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void NotExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NotExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & NotExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprNoOp::isLValue() const {
    return mExpr.isLValue();
}

bool NotExprNoOp::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & NotExprNoOp::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// NotExprLNot
//-----------------------------------------------------------------------------
NotExprLNot::NotExprLNot(NotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

void NotExprLNot::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NotExprLNot::getStartToken() const {
    return mStartToken;
}

const Token & NotExprLNot::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprLNot::isLValue() const {
    return false;
}

bool NotExprLNot::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & NotExprLNot::dataType() const {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kBool);
}

//-----------------------------------------------------------------------------
// NotExprBNot
//-----------------------------------------------------------------------------
NotExprBNot::NotExprBNot(NotExpr & expr, const Token & startToken) :
    mExpr(expr),
    mStartToken(startToken)
{
    mExpr.mParent = this;
}

void NotExprBNot::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & NotExprBNot::getStartToken() const {
    return mStartToken;
}

const Token & NotExprBNot::getEndToken() const {
    return mExpr.getEndToken();
}

bool NotExprBNot::isLValue() const {
    return false;
}

bool NotExprBNot::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & NotExprBNot::dataType() const {
    return mExpr.dataType();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
