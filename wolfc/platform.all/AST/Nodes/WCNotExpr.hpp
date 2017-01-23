#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class CmpExpr;

/*
NotExpr:
	CmpExpr
	not NotExpr
	~ NotExpr
*/
class NotExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static NotExpr * parse(ParseCtx & parseCtx);
};

/* CmpExpr */
class NotExprNoOp final : public NotExpr {
public:
    NotExprNoOp(CmpExpr & expr);
    
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
    
    CmpExpr & mExpr;
};

/* not NotExpr */
class NotExprLNot final : public NotExpr {
public:
    NotExprLNot(NotExpr & expr, const Token & startToken);
    
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
    
    NotExpr &       mExpr;
    const Token &   mStartToken;

#warning FIXME - Codegen
#if 0
private:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error if the expr is not of 'bool'
     * Return false for failure if that is not the case.
     */
    bool compileCheckExprIsBool() const;
#endif
};

/* ~ NotExpr */
class NotExprBNot final : public NotExpr {
public:
    NotExprBNot(NotExpr & expr, const Token & startToken);

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
    
    NotExpr &       mExpr;
    const Token &   mStartToken;

private:
    
#warning FIXME - Codegen
#if 0
    /**
     * TODO: this is a temp function for the moment. Issue a compile error if the expr is not of 'int'
     * Return false for failure if that is not the case.
     */
    bool compileCheckExprIsInt() const;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
