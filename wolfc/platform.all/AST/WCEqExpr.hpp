#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class RelExpr;

/*
EqExpr:
	RelExpr
	RelExpr == EqExpr
	RelExpr != EqExpr
*/
class EqExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static EqExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* RelExpr */
class EqExprNoOp final : public EqExpr {
public:
    EqExprNoOp(RelExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * getStorage() const override;
    virtual void setStorage(llvm::Value & storage) override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    RelExpr & mExpr;
};

/* Base for equality expressions with two operators. */
class EqExprTwoOps : public EqExpr {
public:
    EqExprTwoOps(RelExpr & leftExpr, EqExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    RelExpr & mLeftExpr;
    EqExpr & mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* RelExpr == EqExpr */
class EqExprEq final : public EqExprTwoOps {
public:
    EqExprEq(RelExpr & leftExpr, EqExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* RelExpr != EqExpr */
class EqExprNeq final : public EqExprTwoOps {
public:
    EqExprNeq(RelExpr & leftExpr, EqExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
