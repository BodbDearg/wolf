#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

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
    
    const Token &       mStartToken;
    FuncCallArgList *   mArgList;
    const Token &       mEndToken;
};

WC_END_NAMESPACE
