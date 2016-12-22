#pragma once

#include "DataType/WCDataTypeCodegenFuncs.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AddExpr;
class LinearAlloc;

/*
CmpExpr:
	AddExpr
	AddExpr == CmpExpr
	AddExpr is CmpExpr
	AddExpr != CmpExpr
	AddExpr is not CmpExpr
	AddExpr < CmpExpr
	AddExpr <= CmpExpr
	AddExpr > CmpExpr
	AddExpr >= CmpExpr
*/
class CmpExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static CmpExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* AddExpr */
class CmpExprNoOp final : public CmpExpr {
public:
    CmpExprNoOp(AddExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    AddExpr & mExpr;
};

/* Base for relational expressions with two operators. */
class CmpExprTwoOps : public CmpExpr {
public:
    CmpExprTwoOps(AddExpr & leftExpr,
                  CmpExpr & rightExpr,
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
    
    AddExpr &   mLeftExpr;
    CmpExpr &   mRightExpr;
    
private:
    const DTCodegenBinaryOpFunc         mCodegenBinaryOpFunc;
    const DTCodegenConstBinaryOpFunc    mCodegenConstBinaryOpFunc;
};

/**
 * AddExpr == CmpExpr
 * AddExpr is CmpExpr
 */
class CmpExprEQ final : public CmpExprTwoOps {
public:
    CmpExprEQ(AddExpr & leftExpr, CmpExpr & rightExpr);
};

/** 
 * AddExpr != CmpExpr
 * AddExpr is not CmpExpr
 */
class CmpExprNE final : public CmpExprTwoOps {
public:
    CmpExprNE(AddExpr & leftExpr, CmpExpr & rightExpr);
};

/* AddExpr < CmpExpr */
class CmpExprLT final : public CmpExprTwoOps {
public:
    CmpExprLT(AddExpr & leftExpr, CmpExpr & rightExpr);
};

/* AddExpr <= CmpExpr */
class CmpExprLE final : public CmpExprTwoOps {
public:
    CmpExprLE(AddExpr & leftExpr, CmpExpr & rightExpr);
};

/* AddExpr > CmpExpr */
class CmpExprGT final : public CmpExprTwoOps {
public:
    CmpExprGT(AddExpr & leftExpr, CmpExpr & rightExpr);
};

/* AddExpr >= CmpExpr */
class CmpExprGE final : public CmpExprTwoOps {
public:
    CmpExprGE(AddExpr & leftExpr, CmpExpr & rightExpr);
};

WC_END_NAMESPACE
