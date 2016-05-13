#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class PrintExprs;

/*
Scope:
    PrintExprs
*/
class Scope : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Scope * parse(const Token *& tokenPtr);
    
    Scope(PrintExprs & exprs);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrintExprs & mExprs;
};

WC_END_NAMESPACE
