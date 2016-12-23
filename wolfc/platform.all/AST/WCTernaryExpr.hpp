#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class LinearAlloc;
class LOrExpr;

/*
TernaryExpr:
	LOrExpr
	LOrExpr ? AssignExpr : AssignExpr
*/
class TernaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static TernaryExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* LOrExpr */
class TernaryExprNoCond final : public TernaryExpr {
public:
    TernaryExprNoCond(LOrExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    LOrExpr & mExpr;
};

/* LOrExpr ? AssignExpr : AssignExpr */
class TernaryExprWithCond final : public TernaryExpr {
public:
    TernaryExprWithCond(LOrExpr & condExpr,
                        AssignExpr & trueExpr,
                        AssignExpr & falseExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    /**
     * TODO: this is a temp function for the moment. Verify the types of the sub-expressions
     * involved in this expression. The leftmost expression should be a bool and the two other value 
     * expressions should equate to the same type.
     */
    bool compileCheckExprDataTypes() const;
    
    LOrExpr &       mCondExpr;
    AssignExpr &    mTrueExpr;
    AssignExpr &    mFalseExpr;
};

WC_END_NAMESPACE
