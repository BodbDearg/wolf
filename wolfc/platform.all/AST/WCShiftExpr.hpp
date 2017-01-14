#pragma once

#include "DataType/WCDataTypeCodegenFuncs.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class UnaryExpr;

/*
ShiftExpr:
	UnaryExpr << ShiftExpr
	UnaryExpr >> ShiftExpr
	UnaryExpr >>> ShiftExpr
*/
class ShiftExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ShiftExpr * parse(ParseCtx & parseCtx);
};

/* UnaryExpr */
class ShiftExprNoOp final : public ShiftExpr {
public:
    ShiftExprNoOp(UnaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    UnaryExpr & mExpr;
};

/* Base class for an ShiftExpr with two operands */
class ShiftExprTwoOps : public ShiftExpr {
public:
    ShiftExprTwoOps(UnaryExpr & leftExpr,
                    ShiftExpr & rightExpr,
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
    
    UnaryExpr & mLeftExpr;
    ShiftExpr & mRightExpr;
    
private:
    const DTCodegenBinaryOpFunc         mCodegenBinaryOpFunc;
    const DTCodegenConstBinaryOpFunc    mCodegenConstBinaryOpFunc;
};

/* UnaryExpr << ShiftExpr */
class ShiftExprLShift final : public ShiftExprTwoOps {
public:
    ShiftExprLShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
};

/* UnaryExpr >> ShiftExpr */
class ShiftExprARShift final : public ShiftExprTwoOps {
public:
    ShiftExprARShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
};

/* UnaryExpr >>> ShiftExpr */
class ShiftExprLRShift final : public ShiftExprTwoOps {
public:
    ShiftExprLRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
