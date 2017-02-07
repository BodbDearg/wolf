#include "CmpExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AddExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CmpExpr
//-----------------------------------------------------------------------------
bool CmpExpr::peek(const Token * tokenPtr) {
    return AddExpr::peek(tokenPtr);
}

CmpExpr * CmpExpr::parse(ParseCtx & parseCtx) {
    AddExpr * leftExpr = AddExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);
    
    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            parseCtx.nextTok();\
            CmpExpr * rightExpr = CmpExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    TokenType nextTokType = parseCtx.tok()->type;
    
    switch (nextTokType) {
        PARSE_OP(TokenType::kCmpEQ, CmpExprEQ)  // ==
        PARSE_OP(TokenType::kCmpNE, CmpExprNE)  // !=
        PARSE_OP(TokenType::kCmpLT, CmpExprLT)  // <
        PARSE_OP(TokenType::kCmpLE, CmpExprLE)  // <=
        PARSE_OP(TokenType::kCmpGT, CmpExprGT)  // >
        PARSE_OP(TokenType::kCmpGE, CmpExprGE)  // >=
            
        default:
            break;
    }
    
    // 'is' and 'is not' are just aliases for '==' and '!='.
    // Check for them here now
    if (nextTokType == TokenType::kIs) {
        // Skip 'is'
        parseCtx.nextTok();
        
        // See if 'not' follows, if it does then it inverses the comparison:
        bool cmpNotEq = false;
        
        if (parseCtx.tok()->type == TokenType::kNot) {
            cmpNotEq = true;
            parseCtx.nextTok();
        }
        
        // Parse the right expression
        CmpExpr * rightExpr = CmpExpr::parse(parseCtx);
        WC_GUARD(rightExpr, nullptr);
        
        // Now return the operation:
        if (cmpNotEq) {
            return WC_NEW_AST_NODE(parseCtx, CmpExprNE, *leftExpr, *rightExpr);
        }
        else {
            return WC_NEW_AST_NODE(parseCtx, CmpExprEQ, *leftExpr, *rightExpr);
        }
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(parseCtx, CmpExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// CmpExprNoOp
//-----------------------------------------------------------------------------
CmpExprNoOp::CmpExprNoOp(AddExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void CmpExprNoOp::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & CmpExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & CmpExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// CmpExprTwoOps
//-----------------------------------------------------------------------------
CmpExprTwoOps::CmpExprTwoOps(AddExpr & leftExpr, CmpExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & CmpExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & CmpExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// CmpExprEQ
//-----------------------------------------------------------------------------
CmpExprEQ::CmpExprEQ(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CmpExprEQ::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// CmpExprNE
//-----------------------------------------------------------------------------
CmpExprNE::CmpExprNE(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CmpExprNE::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// CmpExprLT
//-----------------------------------------------------------------------------
CmpExprLT::CmpExprLT(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CmpExprLT::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// CmpExprLE
//-----------------------------------------------------------------------------
CmpExprLE::CmpExprLE(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CmpExprLE::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// CmpExprGT
//-----------------------------------------------------------------------------
CmpExprGT::CmpExprGT(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CmpExprGT::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// CmpExprGE
//-----------------------------------------------------------------------------
CmpExprGE::CmpExprGE(AddExpr & leftExpr, CmpExpr & rightExpr) :
    CmpExprTwoOps(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void CmpExprGE::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
