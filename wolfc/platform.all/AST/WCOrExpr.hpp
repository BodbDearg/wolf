#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AndExpr;
class DataType;
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
class OrExprNoOp : public OrExpr {
public:
    OrExprNoOp(AndExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    AndExpr & mExpr;
};

/* AndExpr or OrExpr */
class OrExprOr : public OrExpr {
public:
    OrExprOr(AndExpr & leftExpr, OrExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    AndExpr & mLeftExpr;
    OrExpr & mRightExpr;
};

WC_END_NAMESPACE
