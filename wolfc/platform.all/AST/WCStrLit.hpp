#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

/* Represents TokenType::kStrLit in the AST tree */
class StrLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static StrLit * parse(ASTNode & parent, const Token *& tokenPtr);
    
    StrLit(ASTNode & parent, const Token & token);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
