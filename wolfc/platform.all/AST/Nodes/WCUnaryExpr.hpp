#pragma once

#warning FIXME - Codegen
#if 0
#include "DataType/WCDataTypeCodegenFuncs.hpp"
#endif

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
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
    static UnaryExpr * parse(ParseCtx & parseCtx);
};

/* PostfixExpr */
class UnaryExprPrimary : public UnaryExpr {
public:
    UnaryExprPrimary(PostfixExpr & expr);
    
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
    
    PostfixExpr & mExpr;
};

/* Base class for '+' or '-' unary expression */
class UnaryExprPlusMinusBase : public UnaryExprPrimary {
public:
    UnaryExprPlusMinusBase(PostfixExpr & expr,
                           const Token & startToken
                        #warning FIXME - Codegen
                        #if 0
                           ,DTCodegenUnaryOpFunc codegenUnaryOpFunc,
                           DTCodegenConstUnaryOpFunc codegenConstUnaryOpFunc
                        #endif
                           );
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) final override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) final override;
#endif
    
    const Token &                       mStartToken;
    
#warning FIXME - Codegen
#if 0
    const DTCodegenUnaryOpFunc          mCodegenUnaryOpFunc;
    const DTCodegenConstUnaryOpFunc     mCodegenConstUnaryOpFunc;
#endif
};

/* + PostfixExpr */
class UnaryExprPlus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprPlus(PostfixExpr & expr, const Token & startToken);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

/* - PostfixExpr */
class UnaryExprMinus final : public UnaryExprPlusMinusBase {
public:
    UnaryExprMinus(PostfixExpr & expr, const Token & startToken);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

/* ( AssignExpr ) */
class UnaryExprParen final : public UnaryExpr {
public:
    UnaryExprParen(AssignExpr & expr, const Token & startToken, const Token & endToken);
    
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

    AssignExpr &    mExpr;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
