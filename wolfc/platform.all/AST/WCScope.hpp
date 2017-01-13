#pragma once

#include "DataType/WCDataValue.hpp"
#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"
#include "WCIStmnt.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <map>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class Stmnt;
class VarDecl;

// TODO: not a statement, don't inherit from IStmnt

/*
Scope:
    Stmnts
*/
class Scope final : public ASTNode, public IStmnt {
public:
    static Scope * parse(ParseCtx & parseCtx);
    
    Scope(const Token & startToken, std::vector<Stmnt*> && stmnts);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
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
    
private:
    /* All the statements in the scope. */
    std::vector<Stmnt*> mStmnts;
    
    /* A list of variables in the scope. Generated during code generation. */
    std::map<const char*, DataValue, CStrComparator> mVarValues;
    
    /* The start token that the scope started off with */
    const Token & mStartToken;
};

WC_END_NAMESPACE
