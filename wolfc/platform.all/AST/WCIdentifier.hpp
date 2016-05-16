#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

/* Represents TokenType::kIdentifier in the AST tree */
class Identifier : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Identifier * parse(const Token *& tokenPtr);
    
    Identifier(const Token & token);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
