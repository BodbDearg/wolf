#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

class CodegenCtx;
class Func;
class LinearAlloc;

/*
DeclDef:
	Func
*/
class DeclDef : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static DeclDef * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    virtual bool codegen(CodegenCtx & cgCtx) = 0;
};

/* Func */
class DeclDefFunc : public DeclDef {
public:
    DeclDefFunc(Func & declDef);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    Func & mFunc;
};

WC_END_NAMESPACE
