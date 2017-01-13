#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;
class NotExpr;

/*
LAndExpr:
	NotExpr
	NotExpr and LAndExpr
*/
class LAndExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static LAndExpr * parse(ParseCtx & parseCtx);
};

/* NotExpr */
class LAndExprNoOp final : public LAndExpr {
public:
    LAndExprNoOp(NotExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    NotExpr & mExpr;
};

/* NotExpr and AndExpr */
class LAndExprAnd final : public LAndExpr {
public:
    LAndExprAnd(NotExpr & leftExpr, LAndExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    NotExpr &   mLeftExpr;
    LAndExpr &  mRightExpr;
    
private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'bool'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreBool() const;
};

WC_END_NAMESPACE
