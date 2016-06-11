#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class Identifier;

/*
VarDecl:
    var Identifier = AssignExpr
*/
class VarDecl : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static VarDecl * parse(const Token *& tokenPtr);
    
    VarDecl(const Token & token, Identifier & ident, AssignExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(CodegenCtx & cgCtx) override;
    
    const Token &   mStartToken;
    Identifier &    mIdent;
    AssignExpr &    mExpr;
};

WC_END_NAMESPACE
