#pragma once

#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"
#include "WCDataValue.hpp"
#include "WCIStmnt.hpp"
#include <map>

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class Stmnts;

// TODO: not a statement, don't inherit from IStmnt

/*
Scope:
    Stmnts
*/
class Scope : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static Scope * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    Scope(Stmnts & stmnts);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(CodegenCtx & cgCtx) override;
    
    /* Create a variable within this scope. If the variable already exists then creation fails and null is returned. */
    const DataValue * createVariable(const char * variableName,
                                     const DataType & dataType,
                                     CodegenCtx & cgCtx);
    
    /* Get a variable within this scope. Does not create if it does not exist. */
    const DataValue * getVariable(const char * variableName) const;
    
    Stmnts & mStmnts;
    std::map<const char*, DataValue, CStrComparator> mVariableValues;
};

WC_END_NAMESPACE
