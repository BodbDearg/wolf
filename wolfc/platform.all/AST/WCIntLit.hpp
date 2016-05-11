#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

/*
IntLit:
	Digits
*/
class IntLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static IntLit * parse(const Token *& tokenPtr);
    
    IntLit(const Token & token);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
