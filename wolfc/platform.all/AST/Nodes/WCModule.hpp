#pragma once

#warning FIXME - Codegen
#if 0
#include "DataType/WCDataValue.hpp"
#endif

#include "WCAssert.hpp"
#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <map>
    #include <memory>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

struct ParseCtx;

WC_AST_BEGIN_NAMESPACE

class DeclDef;
class Func;
class VarDecl;

/* 
Module:
    [0..N: DeclDef] EOF
*/
class Module final : public ASTNode {
public:
    Module();
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    // TODO: change this to return a new module, similar to other ASTNodes
    /* Parse the code for the module from the given token array */
    bool parse(ParseCtx & parseCtx);
    
#warning FIXME - Codegen
#if 0
    /* Generates the code for the module. Parsing code must have succeeded. */
    bool generateCode();
#endif
    
    /** 
     * Register a given function with the module.
     * Returns false on failure if the function is already registered. 
     */
    bool registerFunc(Func & func);
    
    /* Lookup a module function by name. Returns nullptr if not found. */
    Func * getFunc(const std::string & name) const;
    
#warning FIXME - Codegen
#if 0
    /**
     * Create a variable within this scope.
     * If the variable already exists then creation fails and null is returned.
     */
    DataValue * createVar(const char * varName,
                          DataType & dataType,
                          llvm::Constant * initializer,
                          CodegenCtx & cgCtx,
                          VarDecl & callingNode);
    
    /**
     * Get a variable within this scope or a parent scope. Does not create if it does not exist.
     * Will search parent scopes if not found within this scope.
     */
    DataValue * getVar(const char * varName) const;
#endif
    
private:
    /* All declarations and definitions in the module/ */
    std::vector<DeclDef*> mDeclDefs;
    
    /* A list of registered functions in the module. Generated during the compile phase. */
    std::map<std::string, Func*> mFuncs;
    
#warning FIXME - Codegen
#if 0
    /* A list of registered global variables in the module. Generated during the compile phase. */
    std::map<const char*, DataValue, CStrComparator> mVarValues;
#endif
    
    /* The EOF token */
    const Token * mEOFToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
