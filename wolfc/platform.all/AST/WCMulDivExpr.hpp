#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class ShiftExpr;

/*
MulDivExpr:
	ShiftExpr
	ShiftExpr * MulDivExpr
	ShiftExpr / MulDivExpr
	ShiftExpr % MulDivExpr
*/
class MulDivExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static MulDivExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* ShiftExpr */
class MulDivExprNoOp final : public MulDivExpr {
public:
    MulDivExprNoOp(ShiftExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    ShiftExpr & mExpr;
};

/* Base class for an MulDivExpr with two operands */
class MulDivExprTwoOps : public MulDivExpr {
public:
    MulDivExprTwoOps(ShiftExpr & leftExpr, MulDivExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    ShiftExpr &     mLeftExpr;
    MulDivExpr &    mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of type 'int';
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* ShiftExpr * MulDivExpr */
class MulDivExprMul final : public MulDivExprTwoOps {
public:
    MulDivExprMul(ShiftExpr & leftExpr, MulDivExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr / MulDivExpr */
class MulDivExprDiv final : public MulDivExprTwoOps {
public:
    MulDivExprDiv(ShiftExpr & leftExpr, MulDivExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr % MulDivExpr */
class MulDivExprMod final : public MulDivExprTwoOps {
public:
    MulDivExprMod(ShiftExpr & leftExpr, MulDivExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
