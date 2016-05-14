#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class Exprs;

/*
Scope:
    Exprs
*/
class Scope : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Scope * parse(const Token *& tokenPtr);
    
    Scope(Exprs & exprs);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    Exprs & mExprs;
};

WC_END_NAMESPACE
