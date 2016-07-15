#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class NotExpr;

/*
AndExpr:
	NotExpr
	NotExpr and AndExpr
*/
class AndExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static AndExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* NotExpr */
class AndExprNoOp : public AndExpr {
public:
    AndExprNoOp(NotExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    NotExpr & mExpr;
};

/* NotExpr and AndExpr */
class AndExprAnd : public AndExpr {
public:
    AndExprAnd(NotExpr & leftExpr, AndExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    NotExpr & mLeftExpr;
    AndExpr & mRightExpr;
};

WC_END_NAMESPACE