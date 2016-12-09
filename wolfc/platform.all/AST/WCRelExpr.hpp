#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class ShiftExpr;

/*
RelExpr:
	ShiftExpr
	ShiftExpr < RelExpr
	ShiftExpr <= RelExpr
	ShiftExpr > RelExpr
	ShiftExpr >= RelExpr
*/
class RelExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static RelExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* ShiftExpr */
class RelExprNoOp final : public RelExpr {
public:
    RelExprNoOp(ShiftExpr & expr);
    
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
class RelExprTwoOps : public RelExpr {
public:
    RelExprTwoOps(ShiftExpr & leftExpr, RelExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    ShiftExpr & mLeftExpr;
    RelExpr &   mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* ShiftExpr < RelExpr */
class RelExprLT final : public RelExprTwoOps {
public:
    RelExprLT(ShiftExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr <= RelExpr */
class RelExprLE final : public RelExprTwoOps {
public:
    RelExprLE(ShiftExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr > RelExpr */
class RelExprGT final : public RelExprTwoOps {
public:
    RelExprGT(ShiftExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* ShiftExpr >= RelExpr */
class RelExprGE final : public RelExprTwoOps {
public:
    RelExprGE(ShiftExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
