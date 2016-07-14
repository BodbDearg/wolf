#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

class CodegenCtx;
class Identifier;
class LinearAlloc;
class Scope;

/*
Func:
	func Identifier ( ) Scope end
*/
class Func : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static Func * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    Func(const Token & startToken,
         Identifier & identifier,
         Scope & scope,
         const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    bool codegen(CodegenCtx & cgCtx);
    
    const Token &   mStartToken;
    Identifier &    mIdentifier;
    Scope &         mScope;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
