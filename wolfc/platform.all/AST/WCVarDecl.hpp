#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class Identifier;

/*
VarDecl:
    var Identifier = AssignExpr
*/
class VarDecl : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static VarDecl * parse(const Token *& tokenPtr);
    
    VarDecl(Identifier & ident, AssignExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    Identifier & mIdent;
    AssignExpr & mExpr;
};

WC_END_NAMESPACE
