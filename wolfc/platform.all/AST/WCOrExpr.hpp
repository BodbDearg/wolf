#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AndExpr;
class LinearAlloc;

/*
OrExpr:
	AndExpr
	AndExpr or OrExpr
*/
class OrExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static OrExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* AndExpr */
class OrExprNoOp final : public OrExpr {
public:
    OrExprNoOp(AndExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    AndExpr & mExpr;
};

/* AndExpr or OrExpr */
class OrExprOr final : public OrExpr {
public:
    OrExprOr(AndExpr & leftExpr, OrExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    AndExpr &   mLeftExpr;
    OrExpr &    mRightExpr;
    
private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'bool'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreBool() const;
};

WC_END_NAMESPACE
