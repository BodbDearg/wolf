#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class BXorExpr;
class DataType;
class LinearAlloc;

/*
BOrExpr:
	BitXorExpr
	BitXorExpr | BOrExpr
*/
class BOrExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static BOrExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* BitXorExpr */
class BOrExprNoOp final : public BOrExpr {
public:
    BOrExprNoOp(BXorExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    BXorExpr & mExpr;
};

/* BitXorExpr | BOrExpr */
class BOrExprOr final : public BOrExpr {
public:
    BOrExprOr(BXorExpr & leftExpr, BOrExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    BXorExpr &  mLeftExpr;
    BOrExpr &   mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of type 'int';
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

WC_END_NAMESPACE
