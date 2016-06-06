#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class AddSubExpr;

/*
RelExpr:
	AddSubExpr
	AddSubExpr < RelExpr
	AddSubExpr <= RelExpr
	AddSubExpr > RelExpr
	AddSubExpr >= RelExpr
*/
class RelExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static RelExpr * parse(const Token *& tokenPtr);
};

/* AddSubExpr */
class RelExprNoOp : public RelExpr {
public:
    RelExprNoOp(AddSubExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    AddSubExpr & mExpr;
};

/* Base for relational expressions with two operators. */
class RelExprTwoOps : public RelExpr {
public:
    RelExprTwoOps(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    AddSubExpr & mLeftExpr;
    RelExpr & mRightExpr;
};

/* AddSubExpr < RelExpr */
class RelExprLT : public RelExprTwoOps {
public:
    RelExprLT(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
};

/* AddSubExpr <= RelExpr */
class RelExprLE : public RelExprTwoOps {
public:
    RelExprLE(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
};

/* AddSubExpr > RelExpr */
class RelExprGT : public RelExprTwoOps {
public:
    RelExprGT(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
};

/* AddSubExpr >= RelExpr */
class RelExprGE : public RelExprTwoOps {
public:
    RelExprGE(AddSubExpr & leftExpr, RelExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
