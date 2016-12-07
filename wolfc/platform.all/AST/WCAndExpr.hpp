#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class BXorExpr;
class DataType;
class LinearAlloc;

/*
AndExpr:
	BXorExpr
	BXorExpr and AndExpr
*/
class AndExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static AndExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* BAndExpr */
class AndExprNoOp final : public AndExpr {
public:
    AndExprNoOp(BXorExpr & expr);
    
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

/* BAndExpr and AndExpr */
class AndExprAnd final : public AndExpr {
public:
    AndExprAnd(BXorExpr & leftExpr, AndExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    BXorExpr &  mLeftExpr;
    AndExpr &   mRightExpr;
    
private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'bool'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreBool() const;
};

WC_END_NAMESPACE
