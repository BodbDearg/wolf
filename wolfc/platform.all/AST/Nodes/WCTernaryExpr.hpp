#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class LOrExpr;

/*
TernaryExpr:
	LOrExpr
	LOrExpr ? AssignExpr : AssignExpr
*/
class TernaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static TernaryExpr * parse(ParseCtx & parseCtx);
};

/* LOrExpr */
class TernaryExprNoCond final : public TernaryExpr {
public:
    TernaryExprNoCond(LOrExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    LOrExpr & mExpr;
};

/* LOrExpr ? AssignExpr : AssignExpr */
class TernaryExprWithCond final : public TernaryExpr {
public:
    TernaryExprWithCond(LOrExpr & condExpr,
                        AssignExpr & trueExpr,
                        AssignExpr & falseExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    /**
     * TODO: this is a temp function for the moment. Verify the types of the sub-expressions
     * involved in this expression. The leftmost expression should be a bool and the two other value 
     * expressions should equate to the same type.
     */
    bool compileCheckExprDataTypes() const;
#endif
    
    LOrExpr &       mCondExpr;
    AssignExpr &    mTrueExpr;
    AssignExpr &    mFalseExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
