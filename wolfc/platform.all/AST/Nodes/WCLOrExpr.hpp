#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class LAndExpr;

/*
LOrExpr:
	LAndExpr
	LAndExpr or LOrExpr
*/
class LOrExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static LOrExpr * parse(ParseCtx & parseCtx);
};

/* LAndExpr */
class LOrExprNoOp final : public LOrExpr {
public:
    LOrExprNoOp(LAndExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    LAndExpr & mExpr;
};

/* LAndExpr or LOrExpr */
class LOrExprOr final : public LOrExpr {
public:
    LOrExprOr(LAndExpr & leftExpr, LOrExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;

#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    LAndExpr &  mLeftExpr;
    LOrExpr &   mRightExpr;
    
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
