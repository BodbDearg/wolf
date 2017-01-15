#pragma once

#warning FIXME - Codegen
#if 0
#include "DataType/WCDataValue.hpp"
#endif

#include "WCASTNode.hpp"
#include "WCCStrComparator.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
    #include <map>
WC_THIRD_PARTY_INCLUDES_END

#warning FIXME - Codegen
#if 0
namespace llvm {
    class Type;
    class Function;
}
#endif

WC_BEGIN_NAMESPACE

#warning FIXME - Codegen
#if 0
struct CodegenCtx;
#endif

class DataType;

WC_AST_BEGIN_NAMESPACE

class FuncArg;
class FuncArgList;
class Identifier;
class Scope;
class Type;

/*
Func:
	func Identifier ( [FuncArgList] ) [ -> Type ] Scope end
*/
class Func final : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static Func * parse(ParseCtx & parseCtx);
    
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
    void getArgs(std::vector<FuncArg*> & args);
    
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
#endif
    
    /**
     * Checks for duplicate argument names in the given args list and issues a compile
     * error if dupes are found. Returns false if the check fails.
     */
    bool compileCheckForDuplicateArgNames(const std::vector<FuncArg*> & funcArgs) const;
    
#warning FIXME - Codegen
#if 0
    /**
     * Get a list of llvm argument types for this function and save in the given list.
     * Returns false on failure and issues a compile error.
     */
    bool getLLVMArgTypes(const std::vector<FuncArg*> & funcArgs,
                         std::vector<llvm::Type*> & outputArgTypes) const;
#endif
    
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
