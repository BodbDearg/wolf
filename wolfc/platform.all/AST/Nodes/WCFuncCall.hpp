#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class FuncCallArgList;

/*
FuncCall:
	( [FuncCallArgList] )
*/
class FuncCall final : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static FuncCall * parse(ParseCtx & parseCtx);
    
    FuncCall(const Token & startToken, FuncCallArgList * argList, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    size_t numArgs() const;
    void getArgs(std::vector<AssignExpr*> & args) const;
    
#warning FIXME - Codegen
#if 0
    /**
     * Generates the code for the argument list expressions and saves them as a list of 
     * llvm::Value objects on this object.
     */
    bool codegenArgsListExprs(CodegenCtx & cgCtx);
#endif
    
    const Token &       mStartToken;
    FuncCallArgList *   mArgList;
    const Token &       mEndToken;
    
#warning FIXME - Codegen
#if 0
    /* The list of evaluated arg list expressions, created after code generation. */
    std::vector<llvm::Value*> mArgListExprsValues;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
