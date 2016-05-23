#pragma once

#include "WCASTNodeCodegen.hpp"
#include "WCCStrComparator.hpp"
#include <map>

WC_BEGIN_NAMESPACE

class Stmnts;

/*
Scope:
    Stmnts
*/
class Scope : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Scope * parse(const Token *& tokenPtr);
    
    Scope(Stmnts & stmnts);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    /* Get or create a variable within this scope */
    llvm::Value * getOrCreateVariable(const char * variableName,
                                      const CodegenCtx & cgCtx);
    
    /* Get a variable within this scope. Does not create if it does not exist. */
    llvm::Value * getVariable(const char * variableName) const;
    
    Stmnts & mStmnts;
    std::map<const char*, llvm::Value*, CStrComparator> mVariableValues;
};

WC_END_NAMESPACE
