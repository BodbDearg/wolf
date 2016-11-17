#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class EqExpr;
class LinearAlloc;

/*
NotExpr:
	EqExpr
	not NotExpr
*/
class NotExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static NotExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* EqExpr */
class NotExprNoOp final : public NotExpr {
public:
    NotExprNoOp(EqExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * getStorage() const override;
    virtual void setStorage(llvm::Value & storage) override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    EqExpr & mExpr;
};

/* not EqExpr */
class NotExprNot final : public NotExpr {
public:
    NotExprNot(NotExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    NotExpr &       mExpr;
    const Token &   mStartToken;

private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error if the expr is not of 'bool'
     * Return false for failure if that is the case.
     */
    bool compileCheckExprIsBool() const;
};

WC_END_NAMESPACE
