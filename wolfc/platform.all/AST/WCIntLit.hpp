#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

/* Represents TokenType::kIntLit in the AST tree */
class IntLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static IntLit * parse(const Token *& tokenPtr);
    
    IntLit(const Token & token);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
