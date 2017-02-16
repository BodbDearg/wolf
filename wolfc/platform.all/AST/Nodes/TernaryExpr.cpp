#include "TernaryExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"
#include "LOrExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// TernaryExpr
//-----------------------------------------------------------------------------
bool TernaryExpr::peek(const Token * tokenPtr) {
    return LOrExpr::peek(tokenPtr);
}

TernaryExpr * TernaryExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    LOrExpr * firstExpr = LOrExpr::parse(parseCtx);
    WC_GUARD(firstExpr, nullptr);
    
    // See if a '?' follows:
    if (parseCtx.tok()->type == TokenType::kQMark) {
        // Alright, consume that '?' and skip any newlines that follow:
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Now parse the 'true' expression:
        AssignExpr * trueExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(trueExpr, nullptr);
        parseCtx.skipNewlines();        // Skip any newlines that follow
        
        // Expect a colon to separate 'true' from false:
        if (parseCtx.tok()->type != TokenType::kColon) {
            parseCtx.error("Expected ':' following 'true' value in ternary expression.");
            return nullptr;
        }
        
        // Consume the ':' and skip any newlines that follow
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Now parse the 'false' expression:
        AssignExpr * falseExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(falseExpr, nullptr);
        
        // All done: return the parsed node:
        return WC_NEW_AST_NODE(parseCtx, TernaryExprWithCond, *firstExpr, *trueExpr, *falseExpr);
    }

    // Ternary expression with no condition:
    return WC_NEW_AST_NODE(parseCtx, TernaryExprNoCond, *firstExpr);
}

//-----------------------------------------------------------------------------
// TernaryExprNoCond
//-----------------------------------------------------------------------------
TernaryExprNoCond::TernaryExprNoCond(LOrExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void TernaryExprNoCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & TernaryExprNoCond::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & TernaryExprNoCond::getEndToken() const {
    return mExpr.getEndToken();
}

//-----------------------------------------------------------------------------
// TernaryExprWithCond
//-----------------------------------------------------------------------------
TernaryExprWithCond::TernaryExprWithCond(LOrExpr & condExpr,
                                         AssignExpr & trueExpr,
                                         AssignExpr & falseExpr)
:
    mCondExpr(condExpr),
    mTrueExpr(trueExpr),
    mFalseExpr(falseExpr)
{
    mCondExpr.mParent = this;
    mTrueExpr.mParent = this;
    mFalseExpr.mParent = this;
}

void TernaryExprWithCond::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & TernaryExprWithCond::getStartToken() const {
    return mCondExpr.getStartToken();
}

const Token & TernaryExprWithCond::getEndToken() const {
    return mFalseExpr.getEndToken();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
