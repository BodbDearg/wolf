#include "AssignExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "LinearAlloc.hpp"
#include "TernaryExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// AssignExpr
//-----------------------------------------------------------------------------
bool AssignExpr::peek(const Token * tokenPtr) {
    return TernaryExpr::peek(tokenPtr);
}

AssignExpr * AssignExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    TernaryExpr * leftExpr = TernaryExpr::parse(parseCtx);
    WC_GUARD(leftExpr, nullptr);

    // See if there is a known operator ahead.
    // If we find a known operator parse the operator token, the right operand and
    // return the AST node for the operation.
    #define PARSE_OP(TokenType, ASTNodeType)\
        case TokenType: {\
            parseCtx.nextTok();\
            AssignExpr * rightExpr = AssignExpr::parse(parseCtx);\
            WC_GUARD(rightExpr, nullptr);\
            return WC_NEW_AST_NODE(parseCtx, ASTNodeType, *leftExpr, *rightExpr);\
        }

    switch (parseCtx.tok()->type) {
        PARSE_OP(TokenType::kAssign, AssignExprAssign)                  // =
        PARSE_OP(TokenType::kAssignBOr, AssignExprAssignBOr)            // |=
        PARSE_OP(TokenType::kAssignBXor, AssignExprAssignBXor)          // ^=
        PARSE_OP(TokenType::kAssignBAnd, AssignExprAssignBAnd)          // &=
        PARSE_OP(TokenType::kAssignAdd, AssignExprAssignAdd)            // +=
        PARSE_OP(TokenType::kAssignSub, AssignExprAssignSub)            // -=
        PARSE_OP(TokenType::kAssignMul, AssignExprAssignMul)            // *=
        PARSE_OP(TokenType::kAssignDiv, AssignExprAssignDiv)            // /=
        PARSE_OP(TokenType::kAssignMod, AssignExprAssignMod)            // %=
        PARSE_OP(TokenType::kAssignLShift, AssignExprAssignLShift)      // <<=
        PARSE_OP(TokenType::kAssignARShift, AssignExprAssignARShift)    // >>=
        PARSE_OP(TokenType::kAssignLRShift, AssignExprAssignLRShift)    // >>>=
            
        default:
            break;
    }
    
    #undef PARSE_OP

    // Assign expression with no assign
    return WC_NEW_AST_NODE(parseCtx, AssignExprNoAssign, *leftExpr);
}

//-----------------------------------------------------------------------------
// AssignExprNoAssign
//-----------------------------------------------------------------------------
AssignExprNoAssign::AssignExprNoAssign(TernaryExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void AssignExprNoAssign::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & AssignExprNoAssign::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & AssignExprNoAssign::getEndToken() const {
    return mExpr.getEndToken();
}

bool AssignExprNoAssign::isLValue() const {
    return mExpr.isLValue();
}

bool AssignExprNoAssign::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & AssignExprNoAssign::dataType() const {
    return mExpr.dataType();
}

//-----------------------------------------------------------------------------
// AssignExprAssignBase
//-----------------------------------------------------------------------------
AssignExprAssignBase::AssignExprAssignBase(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & AssignExprAssignBase::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & AssignExprAssignBase::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool AssignExprAssignBase::isLValue() const {
    return false;
}

bool AssignExprAssignBase::isConstExpr() const {
    // TODO: eventually we could relax this perhaps and allow for full constant evaluation of a
    // function provided it does not rely on something that can externally vary.
    // For now however, you must use functional type programming if you want constant evaluation.
    return false;
}

const DataType & AssignExprAssignBase::dataType() const {
    // TODO: Handle auto type promotion eventually
    return mLeftExpr.dataType();
}

//-----------------------------------------------------------------------------
// AssignExprBinaryOpBase
//-----------------------------------------------------------------------------
AssignExprBinaryOpBase::AssignExprBinaryOpBase(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// AssignExprAssign
//-----------------------------------------------------------------------------
AssignExprAssign::AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprAssignBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssign::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignAdd
//-----------------------------------------------------------------------------
AssignExprAssignAdd::AssignExprAssignAdd(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignAdd::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignSub
//-----------------------------------------------------------------------------
AssignExprAssignSub::AssignExprAssignSub(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignSub::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignBOr
//-----------------------------------------------------------------------------
AssignExprAssignBOr::AssignExprAssignBOr(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignBOr::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignBXor
//-----------------------------------------------------------------------------
AssignExprAssignBXor::AssignExprAssignBXor(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignBXor::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignMul
//-----------------------------------------------------------------------------
AssignExprAssignMul::AssignExprAssignMul(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignMul::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignDiv
//-----------------------------------------------------------------------------
AssignExprAssignDiv::AssignExprAssignDiv(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignDiv::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignMod
//-----------------------------------------------------------------------------
AssignExprAssignMod::AssignExprAssignMod(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignMod::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignBAnd
//-----------------------------------------------------------------------------
AssignExprAssignBAnd::AssignExprAssignBAnd(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignBAnd::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignLShift
//-----------------------------------------------------------------------------
AssignExprAssignLShift::AssignExprAssignLShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignLShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignARShift
//-----------------------------------------------------------------------------
AssignExprAssignARShift::AssignExprAssignARShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignARShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// AssignExprAssignLRShift
//-----------------------------------------------------------------------------
AssignExprAssignLRShift::AssignExprAssignLRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr) :
    AssignExprBinaryOpBase(leftExpr, rightExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void AssignExprAssignLRShift::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
