#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class DataType;
class Identifier;
class LinearAlloc;
class Module;
class Type;

/*
VarDecl:
    var Identifier = AssignExpr
    var Identifier : Type = AssignExpr
*/
class VarDecl : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static VarDecl * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    VarDecl(const Token & startToken,
            Identifier & ident,
            AssignExpr & initExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool codegen(CodegenCtx & cgCtx) final override;
    
    virtual bool allCodepathsHaveUncondRet() const final override;
    
    virtual DataType & dataType() const = 0;
    
    bool codegenAsLocalVar(CodegenCtx & cgCtx, Scope & parentScope);
    bool codegenAsGlobalVar(CodegenCtx & cgCtx);
    
    const Token &   mStartToken;
    Identifier &    mIdent;
    AssignExpr & 	mInitExpr;
};

/* var Identifier = AssignExpr */
class VarDeclInferType final : public VarDecl {
public:
    VarDeclInferType(const Token & startToken,
                     Identifier & ident,
                     AssignExpr & initExpr);
    
    virtual DataType & dataType() const final override;
};

/* var Identifier : Type = AssignExpr */
class VarDeclExplicitType final : public VarDecl {
public:
    VarDeclExplicitType(const Token & startToken,
                        Identifier & ident,
                        Type & type,
                        AssignExpr & initExpr);
    
    virtual DataType & dataType() const final override;
    
    Type & mType;
};

WC_END_NAMESPACE
