#pragma once

#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"
#include "WCDataValue.hpp"
#include <vector>
#include <map>

namespace llvm {
    class Type;
    class Function;
}

WC_BEGIN_NAMESPACE

class CodegenCtx;
class FuncArg;
class FuncArgList;
class Identifier;
class LinearAlloc;
class Scope;
class Type;

/*
Func:
	func Identifier ( [FuncArgList] ) [ -> Type ] Scope end
*/
class Func : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static Func * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    Func(const Token & startToken,
         Identifier & identifier,
         FuncArgList * argList,
         Type * returnType,
         Scope & scope,
         const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    const char * name() const;
    
    size_t numArgs() const;
    
    void getArgs(std::vector<FuncArg*> & args) const;
    
    const DataValue * getArg(const char * argName) const;
    
    const DataType & returnDataType() const;
    
    /* Forward code generation for the function. Just declares the llvm function in the module. */
    bool codegen(CodegenCtx & cgCtx);
    
    /* Deferred code generation for the function. Generate the actual function body. */
    bool deferredCodegen(CodegenCtx & cgCtx);
    
    /**
     * Checks for duplicate argument names in the given args list and issues a compile
     * error if dupes are found. Returns false if the check fails.
     */
    bool compileCheckForDuplicateArgNames(const std::vector<FuncArg*> & funcArgs) const;
    
    /**
     * Get a list of llvm argument types for this function and save in the given list.
     * Returns false on failure and issues a compile error.
     */
    bool determineLLVMArgTypes(CodegenCtx & cgCtx,
                               const std::vector<FuncArg*> & funcArgs,
                               std::vector<llvm::Type*> & outputArgTypes) const;
    
    const Token &       mStartToken;
    Identifier &        mIdentifier;
    FuncArgList *       mArgList;
    Scope &             mScope;
    const Token &       mEndToken;
    
    /**
     * The explicitly specified return type for this function. 
     * If this is null then void type is assumed.
     */
    Type * mReturnType;
    
    /* The llvm function object for this function */
    llvm::Function * mLLVMFunc = nullptr;
    
    /* The values for the parameters passed into the function. Generated during foward code generation. */
    std::map<const char*, DataValue, CStrComparator> mArgValues;
};

WC_END_NAMESPACE
