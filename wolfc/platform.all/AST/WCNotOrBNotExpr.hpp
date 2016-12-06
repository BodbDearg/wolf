#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class EqExpr;
class LinearAlloc;

/*
NotOrBNotExpr:
	EqExpr
	not NotOrBNotExpr
	~ NotOrBNotExpr
*/
class NotOrBNotExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static NotOrBNotExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* EqExpr */
class NotOrBNotExprNoOp final : public NotOrBNotExpr {
public:
    NotOrBNotExprNoOp(EqExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    EqExpr & mExpr;
};

/* not EqExpr */
class NotOrBNotExprNot final : public NotOrBNotExpr {
public:
    NotOrBNotExprNot(NotOrBNotExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    NotOrBNotExpr &     mExpr;
    const Token &       mStartToken;

private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error if the expr is not of 'bool'
     * Return false for failure if that is the case.
     */
    bool compileCheckExprIsBool() const;
};

WC_END_NAMESPACE
