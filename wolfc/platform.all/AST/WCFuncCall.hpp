#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;

/*
FuncCall:
    ()
*/
class FuncCall : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    
    static FuncCall * parse(const Token *& currentToken, LinearAlloc & alloc);
};

/* () */
class FuncCallNoArgs : public FuncCall {
public:
    FuncCallNoArgs(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

WC_END_NAMESPACE
