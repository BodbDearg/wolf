#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

/* Represents TokenType::kBoolLit in the AST tree */
class BoolLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static BoolLit * parse(const Token *& tokenPtr);
    
    BoolLit(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
