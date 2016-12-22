#pragma once

#include "DataType/WCDataTypeCodegenFuncs.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class LinearAlloc;
class PostfixExpr;

/*
UnaryExpr:
	PostfixExpr
	+ PostfixExpr
	- PostfixExpr
	( AssignExpr )
*/
class UnaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static UnaryExpr * parse(const Token *& currentToken, LinearAlloc & alloc);
};

/* PostfixExpr */
class UnaryExprPrimary : public UnaryExpr {
public:
    UnaryExprPrimary(PostfixExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    PostfixExpr & mExpr;
};

/* Base class for '+' or '-' unary expression */
class UnaryExprPlusMinusBase : public UnaryExprPrimary {
public:
    UnaryExprPlusMinusBase(PostfixExpr & expr,
                           const Token & startToken,
                           DTCodegenUnaryOpFunc codegenUnaryOpFunc,
                           DTCodegenConstUnaryOpFunc codegenConstUnaryOpFunc);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) final override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) final override;
    
    const Token &                       mStartToken;
    const DTCodegenUnaryOpFunc          mCodegenUnaryOpFunc;
    const DTCodegenConstUnaryOpFunc     mCodegenConstUnaryOpFunc;
};

/* + PostfixExpr */
class UnaryExprPlus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprPlus(PostfixExpr & expr, const Token & startToken);
};

/* - PostfixExpr */
class UnaryExprMinus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprMinus(PostfixExpr & expr, const Token & startToken);
};

/* ( AssignExpr ) */
class UnaryExprParen final : public UnaryExpr {
public:
    UnaryExprParen(AssignExpr & expr, const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    AssignExpr &    mExpr;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
