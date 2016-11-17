#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class AssignExpr;
class CodegenCtx;
class FuncCallArgList;
class LinearAlloc;

/*
FuncCall:
	( [FuncCallArgList] )
*/
class FuncCall final : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static FuncCall * parse(const Token *& currentToken, LinearAlloc & alloc);
    
    FuncCall(const Token & startToken, FuncCallArgList * argList, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    size_t numArgs() const;
    void getArgs(std::vector<AssignExpr*> & args) const;
    
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
