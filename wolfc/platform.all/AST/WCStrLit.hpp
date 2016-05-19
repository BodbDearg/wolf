#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

/* Represents TokenType::kStrLit in the AST tree */
class StrLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static StrLit * parse(const Token *& tokenPtr);
    
    StrLit(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
