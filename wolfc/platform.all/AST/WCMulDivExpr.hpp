#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class UnaryExpr;

/*
MulDivExpr:
	UnaryExpr
	UnaryExpr * MulDivExpr
	UnaryExpr / MulDivExpr
*/
class MulDivExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static MulDivExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* UnaryExpression */
class MulDivExprNoOp final : public MulDivExpr {
public:
    MulDivExprNoOp(UnaryExpr & expr);
    
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
    
    UnaryExpr & mExpr;
};

/* Base class for an MulDivExpr with two operands */
class MulDivExprTwoOps : public MulDivExpr {
public:
    MulDivExprTwoOps(UnaryExpr & leftExpr, MulDivExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    UnaryExpr & mLeftExpr;
    MulDivExpr & mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* UnaryExpr * MulDivExpr */
class MulDivExprMul final : public MulDivExprTwoOps {
public:
    MulDivExprMul(UnaryExpr & leftExpr, MulDivExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* UnaryExpr / MulDivExpr */
class MulDivExprDiv final : public MulDivExprTwoOps {
public:
    MulDivExprDiv(UnaryExpr & leftExpr, MulDivExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
