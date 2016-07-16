#pragma once

#include "WCASTNode.hpp"
#include <vector>

namespace llvm {
    class Type;
}

WC_BEGIN_NAMESPACE

class CodegenCtx;
class FuncArg;
class FuncArgList;
class Identifier;
class LinearAlloc;
class Scope;

/*
Func:
	func Identifier ( [FuncArgList] ) Scope end
*/
class Func : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static Func * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    Func(const Token & startToken,
         Identifier & identifier,
         FuncArgList * argList,
         Scope & scope,
         const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    bool codegen(CodegenCtx & cgCtx);
    
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
    
    const Token &   mStartToken;
    Identifier &    mIdentifier;
    FuncArgList *   mArgList;
    Scope &         mScope;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
