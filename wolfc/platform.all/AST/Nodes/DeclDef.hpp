//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"

WC_BEGIN_NAMESPACE
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
};

/* Func */
class DeclDefFunc final : public DeclDef {
public:
    DeclDefFunc(Func & declDef);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    Func & mFunc;
};

/* VarDecl */
class DeclDefVarDecl final : public DeclDef {
public:
    DeclDefVarDecl(VarDecl & varDecl);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    VarDecl & mVarDecl;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
