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
    /* Parse the code for the module from the given parse context */
    static Module * parse(ParseCtx & parseCtx);
    
    Module(std::vector<DeclDef*> && mDeclDefs, const Token & eofToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    /* Generates the code for the module. Parsing code must have succeeded. */
    bool generateCode();
#endif
    
#warning FIXME - Codegen
#if 0
    /** 
     * Register a given function with the module.
     * Returns false on failure if the function is already registered. 
     */
    bool registerFunc(Func & func);
    
    /* Lookup a module function by name. Returns nullptr if not found. */
    Func * getFunc(const std::string & name) const;
    
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
    const std::vector<DeclDef*> mDeclDefs;
    
#warning FIXME - Codegen
#if 0
    /* A list of registered functions in the module. Generated during the compile phase. */
    std::map<std::string, Func*> mFuncs;
    
    /* A list of registered global variables in the module. Generated during the compile phase. */
    std::map<const char*, DataValue, CStrComparator> mVarValues;
#endif
    
    /* The EOF token */
    const Token & mEOFToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
