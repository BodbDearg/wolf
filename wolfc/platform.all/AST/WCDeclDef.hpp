#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;

WC_AST_BEGIN_NAMESPACE

class Func;
class VarDecl;

/*
DeclDef:
	Func
	VarDecl
*/
class DeclDef : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static DeclDef * parse(ParseCtx & parseCtx);
    
    virtual bool codegen(CodegenCtx & cgCtx) = 0;
};

/* Func */
class DeclDefFunc final : public DeclDef {
public:
    DeclDefFunc(Func & declDef);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    Func & mFunc;
};

/* VarDecl */
class DeclDefVarDecl final : public DeclDef {
public:
    DeclDefVarDecl(VarDecl & varDecl);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    VarDecl & mVarDecl;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
