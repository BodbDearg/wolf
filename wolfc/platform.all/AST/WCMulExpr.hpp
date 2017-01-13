#pragma once

#include "DataType/WCDataTypeCodegenFuncs.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;
class ShiftExpr;

/*
MulExpr:
	ShiftExpr
	ShiftExpr * MulExpr
	ShiftExpr / MulExpr
	ShiftExpr % MulExpr
*/
class MulExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static MulExpr * parse(ParseCtx & parseCtx);
};

/* ShiftExpr */
class MulExprNoOp final : public MulExpr {
public:
    MulExprNoOp(ShiftExpr & expr);
    
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

/* Base class for an MulExpr with two operands */
class MulExprTwoOps : public MulExpr {
public:
    MulExprTwoOps(ShiftExpr & leftExpr,
                  MulExpr & rightExpr,
                  DTCodegenBinaryOpFunc codegenBinaryOpFunc,
                  DTCodegenConstBinaryOpFunc codegenConstBinaryOpFunc);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    ShiftExpr & mLeftExpr;
    MulExpr &   mRightExpr;
    
private:
    const DTCodegenBinaryOpFunc         mCodegenBinaryOpFunc;
    const DTCodegenConstBinaryOpFunc    mCodegenConstBinaryOpFunc;
};

/* ShiftExpr * MulExpr */
class MulExprMul final : public MulExprTwoOps {
public:
    MulExprMul(ShiftExpr & leftExpr, MulExpr & rightExpr);
};

/* ShiftExpr / MulExpr */
class MulExprDiv final : public MulExprTwoOps {
public:
    MulExprDiv(ShiftExpr & leftExpr, MulExpr & rightExpr);
};

/* ShiftExpr % MulExpr */
class MulExprMod final : public MulExprTwoOps {
public:
    MulExprMod(ShiftExpr & leftExpr, MulExpr & rightExpr);
};

/* ShiftExpr & MulExpr */
class MulExprBAnd final : public MulExprTwoOps {
public:
    MulExprBAnd(ShiftExpr & leftExpr, MulExpr & rightExpr);
};


WC_END_NAMESPACE
