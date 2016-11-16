#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AddSubExpr;
class DataType;
class LinearAlloc;

/*
RelExpr:
	AddSubExpr
	AddSubExpr < RelExpr
	AddSubExpr <= RelExpr
	AddSubExpr > RelExpr
	AddSubExpr >= RelExpr
*/
class RelExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static RelExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* AddSubExpr */
class RelExprNoOp final : public RelExpr {
public:
    RelExprNoOp(AddSubExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool requiresStorage() const override;
    
    virtual llvm::Value * getStorage() const override;
    
    virtual void setStorage(llvm::Value & storage) override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    AddSubExpr & mExpr;
};

/* Base for relational expressions with two operators. */
class RelExprTwoOps : public RelExpr {
public:
    RelExprTwoOps(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() const final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
    
    AddSubExpr & mLeftExpr;
    RelExpr & mRightExpr;
};

/* AddSubExpr < RelExpr */
class RelExprLT final : public RelExprTwoOps {
public:
    RelExprLT(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr <= RelExpr */
class RelExprLE final : public RelExprTwoOps {
public:
    RelExprLE(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr > RelExpr */
class RelExprGT final : public RelExprTwoOps {
public:
    RelExprGT(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr >= RelExpr */
class RelExprGE final : public RelExprTwoOps {
public:
    RelExprGE(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
