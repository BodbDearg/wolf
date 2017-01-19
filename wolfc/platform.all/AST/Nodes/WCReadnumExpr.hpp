#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class StrLit;

/*
ReadnumExpr:
    readnum ( )
*/
class ReadnumExpr final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ReadnumExpr * parse(ParseCtx & parseCtx);
    
    ReadnumExpr(const Token & startToken, const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
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
    
    const Token & mStartToken;
    const Token & mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
