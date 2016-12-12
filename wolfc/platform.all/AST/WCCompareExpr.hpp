#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AddSubExpr;
class DataType;
class LinearAlloc;

/*
CompareExpr:
	AddSubExpr
	AddSubExpr == CompareExpr
	AddSubExpr != CompareExpr
	AddSubExpr < CompareExpr
	AddSubExpr <= CompareExpr
	AddSubExpr > CompareExpr
	AddSubExpr >= CompareExpr
*/
class CompareExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static CompareExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* AddSubExpr */
class CompareExprNoOp final : public CompareExpr {
public:
    CompareExprNoOp(AddSubExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    AddSubExpr & mExpr;
};

/* Base for relational expressions with two operators. */
class CompareExprTwoOps : public CompareExpr {
public:
    CompareExprTwoOps(AddSubExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    AddSubExpr &    mLeftExpr;
    CompareExpr &   mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* AddSubExpr == CompareExpr */
class CompareExprEQ final : public CompareExprTwoOps {
public:
    CompareExprEQ(AddSubExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr != CompareExpr */
class CompareExprNE final : public CompareExprTwoOps {
public:
    CompareExprNE(AddSubExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr < CompareExpr */
class CompareExprLT final : public CompareExprTwoOps {
public:
    CompareExprLT(AddSubExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr <= CompareExpr */
class CompareExprLE final : public CompareExprTwoOps {
public:
    CompareExprLE(AddSubExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr > CompareExpr */
class CompareExprGT final : public CompareExprTwoOps {
public:
    CompareExprGT(AddSubExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr >= CompareExpr */
class CompareExprGE final : public CompareExprTwoOps {
public:
    CompareExprGE(AddSubExpr & leftExpr, CompareExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
