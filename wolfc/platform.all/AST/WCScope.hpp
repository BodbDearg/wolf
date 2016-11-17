#pragma once

#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"
#include "WCDataValue.hpp"
#include "WCIBasicCodegenNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <map>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class Stmnts;
class VarDecl;

// TODO: not a statement, don't inherit from IStmnt

/*
Scope:
    Stmnts
*/
class Scope final : public ASTNode, public IBasicCodegenNode {
public:
    static bool peek(const Token * tokenPtr);
    static Scope * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    Scope(Stmnts & stmnts);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    /** 
     * Create a variable within this scope.
     * If the variable already exists then creation fails and null is returned. 
     */
    DataValue * createVar(const char * varName,
                          DataType & dataType,
                          CodegenCtx & cgCtx,
                          VarDecl & callingNode);
    
    /* Get a variable within this scope. Returns null if not found within this scope. */
    DataValue * getVar(const char * varName);
    
    /* All the statements in the scope. */
    Stmnts & mStmnts;
    
private:
    /* A list of variables in the scope. Generated during code generation. */
    std::map<const char*, DataValue, CStrComparator> mVarValues;
};

WC_END_NAMESPACE
