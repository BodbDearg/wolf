#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class DataType;
class LinearAlloc;
class PostfixExpr;

/*
UnaryExpr:
	PostfixExpr
	-PostfixExpr
	+PostfixExpr
	(AssignExpr)
*/
class UnaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    
    static UnaryExpr * parse(const Token *& currentToken, LinearAlloc & alloc);
};

/* PrimaryExpr */
class UnaryExprPrimary : public UnaryExpr {
public:
    UnaryExprPrimary(PostfixExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    PostfixExpr & mExpr;
};

/* -PrimaryExpr */
class UnaryExprMinus : public UnaryExpr {
public:
    UnaryExprMinus(PostfixExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    PostfixExpr &   mExpr;
    const Token &   mStartToken;
};

/* +PrimaryExpr */
class UnaryExprPlus : public UnaryExprPrimary {
public:
    UnaryExprPlus(PostfixExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    const Token &   mStartToken;
};

/* (AssignExpr) */
class UnaryExprParen : public UnaryExpr {
public:
    UnaryExprParen(AssignExpr & expr, const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    AssignExpr &    mExpr;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
