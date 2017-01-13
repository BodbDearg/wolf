#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class LAndExpr;
class LinearAlloc;

/*
LOrExpr:
	LAndExpr
	LAndExpr or LOrExpr
*/
class LOrExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static LOrExpr * parse(ParseCtx & parseCtx);
};

/* LAndExpr */
class LOrExprNoOp final : public LOrExpr {
public:
    LOrExprNoOp(LAndExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    LAndExpr & mExpr;
};

/* LAndExpr or LOrExpr */
class LOrExprOr final : public LOrExpr {
public:
    LOrExprOr(LAndExpr & leftExpr, LOrExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    LAndExpr &  mLeftExpr;
    LOrExpr &   mRightExpr;
    
private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'bool'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreBool() const;
};

WC_END_NAMESPACE
