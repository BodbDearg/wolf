#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class NotExpr;

/*
LAndExpr:
	NotExpr
	NotExpr and LAndExpr
*/
class LAndExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static LAndExpr * parse(ParseCtx & parseCtx);
};

/* NotExpr */
class LAndExprNoOp final : public LAndExpr {
public:
    LAndExprNoOp(NotExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    NotExpr & mExpr;
};

/* NotExpr and AndExpr */
class LAndExprAnd final : public LAndExpr {
public:
    LAndExprAnd(NotExpr & leftExpr, LAndExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    NotExpr &   mLeftExpr;
    LAndExpr &  mRightExpr;
    
#warning FIXME - Codegen
#if 0
private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'bool'
     * Return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreBool() const;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
