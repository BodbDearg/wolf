#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

/* Holds a simple unsigned integer literal */
class IntLit : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static IntLit * parse(const Token *& tokenPtr);
    
    IntLit(const Token & token);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
