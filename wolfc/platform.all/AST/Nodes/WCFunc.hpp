#pragma once

#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
    #include <map>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class FuncArg;
class Identifier;
class Scope;
class Type;

/*
Func:
	func Identifier ( [0..N: FuncArg ,][FuncArg] ) [-> Type] Scope end
*/
class Func final : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static Func * parse(ParseCtx & parseCtx);
    
    Func(const Token & startToken,
         Identifier & identifier,
         std::vector<FuncArg*> && funcArgs,
         Type * returnType,
         Scope & scope,
         const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const char * name() const;
    
    inline const std::vector<FuncArg*> & getArgs() const {
        return mFuncArgs;
    }
    
#warning FIXME - Codegen
#if 0
    DataValue * getArg(const char * argName);
#endif
    
    DataType & returnDataType() const;
    
#warning FIXME - Codegen
#if 0
    /* Forward code generation for the function. Just declares the llvm function in the module. */
    bool codegen(CodegenCtx & cgCtx);
    
    /**
     * Codegen the return type for the function and return it. Returns nullptr and issues
     * a compile error on failure.
     */
    llvm::Type * codegenLLVMReturnType(CodegenCtx & cgCtx);
    
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
    bool getLLVMArgTypes(const std::vector<FuncArg*> & funcArgs,
                         std::vector<llvm::Type*> & outputArgTypes) const;
#endif
    
    const Token &       mStartToken;
    Identifier &        mIdentifier;
    Scope &             mScope;
    const Token &       mEndToken;
    
private:
    /* The list of function arguments parsed */
    std::vector<FuncArg*> mFuncArgs;
    
    /**
     * The explicitly specified return type for this function. 
     * If this is null then void type is assumed.
     */
    Type * mReturnType;
    
#warning FIXME - Codegen
#if 0
    /* The llvm function object for this function */
    llvm::Function * mLLVMFunc = nullptr;
    
    /* The values for the parameters passed into the function. Generated during foward code generation. */
    std::map<const char*, DataValue, CStrComparator> mArgValues;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
