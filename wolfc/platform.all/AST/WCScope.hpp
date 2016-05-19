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
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    /* Get or create a variable within this scope */
    llvm::Value * getOrCreateVariable(const char32_t * variableName,
                                      const CodegenCtx & cgCtx);
    
    /* Get a variable within this scope. Does not create if it does not exist. */
    llvm::Value * getVariable(const char32_t * variableName) const;
    
    Exprs & mExprs;
    std::map<const char32_t*, llvm::Value*, CStrComparator> mVariableValues;
};

WC_END_NAMESPACE
