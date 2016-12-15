#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;

/* Represents TokenType::kStrLit in the AST tree */
class StrLit final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static StrLit * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    StrLit(const Token & token);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
