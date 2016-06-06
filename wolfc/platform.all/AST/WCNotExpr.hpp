#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AddSubExpr;
class DataType;

/*
NotExpr:
	AddSubExpr
	not NotExpr
*/
class NotExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static NotExpr * parse(const Token *& tokenPtr);
};

/* AddSubExpr */
class NotExprNoOp : public NotExpr {
public:
    NotExprNoOp(AddSubExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    AddSubExpr & mExpr;
};

/* not NotExpr */
class NotExprNot : public NotExpr {
public:
    NotExprNot(NotExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;

    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    NotExpr &       mExpr;
    const Token &   mStartToken;
};

WC_END_NAMESPACE
