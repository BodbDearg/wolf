#pragma once

#include "VisitorMacros.hpp"
#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class DeclDefVisitor;
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
    
    WC_MUST_ACCEPT_VISTOR(DeclDefVisitor);
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) = 0;
#endif
};

/* Func */
class DeclDefFunc final : public DeclDef {
public:
    DeclDefFunc(Func & declDef);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    WC_WILL_ACCEPT_VISITOR(DeclDefVisitor);
    
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
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    WC_WILL_ACCEPT_VISITOR(DeclDefVisitor);
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    VarDecl & mVarDecl;
};

/* Visitor interface */
WC_DECLARE_VISITOR_INTERFACE(DeclDefVisitor,
                             DeclDefFunc,
                             DeclDefVarDecl);

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
