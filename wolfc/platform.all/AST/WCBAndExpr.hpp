#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class NotExpr;

/*
BAndExpr:
    NotExpr
    NotExpr & BAndExpr
*/
class BAndExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static BAndExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* NotExpr */
class BAndExprNoOp final : public BAndExpr {
public:
    BAndExprNoOp(NotExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    NotExpr & mExpr;
};

/* NotExpr & BAndExpr */
class BAndExprBAnd final : public BAndExpr {
public:
    BAndExprBAnd(NotExpr & leftExpr, BAndExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    NotExpr &   mLeftExpr;
    BAndExpr &  mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of type 'int';
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

WC_END_NAMESPACE
