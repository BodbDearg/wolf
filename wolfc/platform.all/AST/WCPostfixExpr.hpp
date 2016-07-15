#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class PrimaryExpr;
class FuncInvocation;

/*
PostfixExpr:
	PrimaryExpr
	PrimaryExpr FuncInvocation
*/
class PostfixExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    
    static PostfixExpr * parse(const Token *& currentToken, LinearAlloc & alloc);
};

/* PrimaryExpr */
class PostfixExprNoPostfix : public PostfixExpr {
public:
    PostfixExprNoPostfix(PrimaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    PrimaryExpr & mExpr;
};

/* PrimaryExpr FuncInvocation */
class PostfixExprFuncInvocation : public PostfixExpr {
public:
    PostfixExprFuncInvocation(PrimaryExpr & expr, FuncInvocation & funcInvocation);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    PrimaryExpr &       mExpr;
    FuncInvocation &    mFuncInvocation;
};

WC_END_NAMESPACE
