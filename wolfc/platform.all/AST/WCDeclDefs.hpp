#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

class CodegenCtx;
class DeclDef;
class LinearAlloc;

/*
DeclDefs:
	DeclDef
	DeclDef DeclDefs
*/
class DeclDefs : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static DeclDefs * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    virtual bool codegen(CodegenCtx & cgCtx) = 0;
};

/* DeclDef */
class DeclDefsSingle : public DeclDefs {
public:
    DeclDefsSingle(DeclDef & declDef);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    DeclDef & mDeclDef;
};

/* DeclDef DeclDefs */
class DeclDefsMulti : public DeclDefs {
public:
    DeclDefsMulti(DeclDef & declDef, DeclDefs & declDefs);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    DeclDef &   mDeclDef;
    DeclDefs &  mDeclDefs;
};

WC_END_NAMESPACE
