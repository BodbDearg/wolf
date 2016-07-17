#pragma once

#include "WCASTNode.hpp"
#include <vector>

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class CodegenCtx;
class FuncCallArgList;
class LinearAlloc;

/*
FuncCall:
	( [FuncCallArgList] )
*/
class FuncCall : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    
    static FuncCall * parse(const Token *& currentToken, LinearAlloc & alloc);
    
    FuncCall(const Token & startToken, FuncCallArgList * argList, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    size_t numArgs() const;
    
    /**
     * Generates the code for the argument list expressions and saves them as a list of 
     * llvm::Value objects on this object.
     */
    bool codegenArgsListExprs(CodegenCtx & cgCtx);
    
    const Token &       mStartToken;
    FuncCallArgList *   mArgList;
    const Token &       mEndToken;
    
    /* The list of evaluated arg list expressions, created after code generation. */
    std::vector<llvm::Value*> mArgListExprsValues;
};

WC_END_NAMESPACE
