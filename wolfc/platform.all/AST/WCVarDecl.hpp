#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class Identifier;
class LinearAlloc;
class Module;
class TernaryExpr;

/*
VarDecl:
    var Identifier = TernaryExpr
*/
class VarDecl final : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static VarDecl * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    VarDecl(const Token & token, Identifier & ident, TernaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    bool codegenAsLocalVar(CodegenCtx & cgCtx, Scope & parentScope);
    bool codegenAsGlobalVar(CodegenCtx & cgCtx);
    
    Identifier &    mIdent;
    TernaryExpr &   mExpr;
    
private:
    const Token & mStartToken;
};

WC_END_NAMESPACE
