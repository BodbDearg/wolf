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
        // Alright, consume that '?':
        parseCtx.nextTok();
        
        // Now parse the 'true' expression:
        AssignExpr * trueExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(trueExpr, nullptr);
        
        // Expect a colon to separate 'true' from false:
        if (parseCtx.tok()->type != TokenType::kColon) {
            parseCtx.error("Expected ':' following 'true' value in ternary expression.");
            return nullptr;
        }
        
        // Consume the ':'
        parseCtx.nextTok();
        
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

bool TernaryExprNoCond::isLValue() const {
    return mExpr.isLValue();
}

bool TernaryExprNoCond::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & TernaryExprNoCond::dataType() const {
    return mExpr.dataType();
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

bool TernaryExprWithCond::isLValue() const {
    return false;
}

bool TernaryExprWithCond::isConstExpr() const {
    return  mCondExpr.isConstExpr() &&
            mTrueExpr.isConstExpr() &&
            mFalseExpr.isConstExpr();
}

const DataType & TernaryExprWithCond::dataType() const {
    // Assume the data type is that of the 'true' expression.
    // When we compile we will verify that the 'false' expression matches...
    return mTrueExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Constant * TernaryExprWithCond::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Verify data types used by operator:
    WC_GUARD(compileCheckExprDataTypes(), nullptr);
    
    // Generate the code for the boolean condition:
    llvm::Constant * condValue = mCondExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(condValue, nullptr);
    
    // See whether the value is true or false: use that to decide which sub expression to choose
    if (condValue->isZeroValue()) {
        return mFalseExpr.codegenExprConstEval(cgCtx);
    }
    
    return mTrueExpr.codegenExprConstEval(cgCtx);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
