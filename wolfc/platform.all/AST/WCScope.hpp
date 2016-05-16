#pragma once

#include "WCASTNodeCodegen.hpp"
#include "WCCStrComparator.hpp"
#include <map>

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
    std::map<char32_t*, llvm::Value*, CStrComparator> mVariableValues;
};

WC_END_NAMESPACE
