#pragma once

#include "WCASTNode.hpp"

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
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) = 0;
#endif
};

/* Func */
class DeclDefFunc final : public DeclDef {
public:
    DeclDefFunc(Func & declDef);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    Func & mFunc;
};

/* VarDecl */
class DeclDefVarDecl final : public DeclDef {
public:
    DeclDefVarDecl(VarDecl & varDecl);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    VarDecl & mVarDecl;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
