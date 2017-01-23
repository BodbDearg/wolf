#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

// TODO: Make this be for ALL numeric literals

/* Represents TokenType::kIntLit in the AST tree */
class IntLit final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static IntLit * parse(ParseCtx & parseCtx);
    
    IntLit(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
