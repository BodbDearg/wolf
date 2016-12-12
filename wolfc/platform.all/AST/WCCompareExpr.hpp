#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class ShiftExpr;

/*
CompareExpr:
	ShiftExpr
	ShiftExpr == CompareExpr
	ShiftExpr != CompareExpr
	ShiftExpr < CompareExpr
	ShiftExpr <= CompareExpr
	ShiftExpr > CompareExpr
	ShiftExpr >= CompareExpr
*/
class CompareExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static CompareExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* ShiftExpr */
class CompareExprNoOp final : public CompareExpr {
public:
    CompareExprNoOp(ShiftExpr & expr);
    
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

/* Base for relational expressions with two operators. */
class CompareExprTwoOps : public CompareExpr {
public:
    CompareExprTwoOps(ShiftExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    ShiftExpr &     mLeftExpr;
    CompareExpr &   mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* ShiftExpr == CompareExpr */
class CompareExprEQ final : public CompareExprTwoOps {
public:
    CompareExprEQ(ShiftExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr != CompareExpr */
class CompareExprNE final : public CompareExprTwoOps {
public:
    CompareExprNE(ShiftExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr < CompareExpr */
class CompareExprLT final : public CompareExprTwoOps {
public:
    CompareExprLT(ShiftExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr <= CompareExpr */
class CompareExprLE final : public CompareExprTwoOps {
public:
    CompareExprLE(ShiftExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr > CompareExpr */
class CompareExprGT final : public CompareExprTwoOps {
public:
    CompareExprGT(ShiftExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr >= CompareExpr */
class CompareExprGE final : public CompareExprTwoOps {
public:
    CompareExprGE(ShiftExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
