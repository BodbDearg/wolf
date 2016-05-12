#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

/* Represents TokenType::kStrLit in the AST tree */
class StrLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static StrLit * parse(const Token *& tokenPtr);
    
    StrLit(const Token & token);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
