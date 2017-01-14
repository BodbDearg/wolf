#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class PrimaryExpr;
class Type;

/*
CastExpr:
	PrimaryExpr
	cast ( AssignExpr to Type )
*/
class CastExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static CastExpr * parse(ParseCtx & parseCtx);
};

/* PrimaryExpr */
class CastExprNoCast final : public CastExpr {
public:
    CastExprNoCast(PrimaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    PrimaryExpr & mExpr;
};

/* cast ( AssignExpr to Type ) */
class CastExprCast final : public CastExpr {
public:
    CastExprCast(const Token & startToken,
                 AssignExpr & expr,
                 Type & type,
                 const Token & endToken);

    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;

    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;

    virtual DataType & dataType() final override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;

    const Token &   mStartToken;
    AssignExpr &    mExpr;
    Type &          mType;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
