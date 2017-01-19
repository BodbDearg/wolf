#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class Identifier;
class Module;
class Type;

/*
VarDecl:
    let Identifier = AssignExpr
    let Identifier : Type = AssignExpr
*/
class VarDecl : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static VarDecl * parse(ParseCtx & parseCtx);
    
    VarDecl(const Token & startToken,
            Identifier & ident,
            AssignExpr & initExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) final override;
#endif
    
    virtual bool allCodepathsHaveUncondRet() const final override;
    
    virtual DataType & dataType() const = 0;
    
#warning FIXME - Codegen
#if 0
    bool codegenAsLocalVar(CodegenCtx & cgCtx, Scope & parentScope);
    bool codegenAsGlobalVar(CodegenCtx & cgCtx);
#endif
    
    const Token &   mStartToken;
    Identifier &    mIdent;
    AssignExpr & 	mInitExpr;
};

/* let Identifier = AssignExpr */
class VarDeclInferType final : public VarDecl {
public:
    VarDeclInferType(const Token & startToken,
                     Identifier & ident,
                     AssignExpr & initExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    virtual DataType & dataType() const final override;
};

/* let Identifier : Type = AssignExpr */
class VarDeclExplicitType final : public VarDecl {
public:
    VarDeclExplicitType(const Token & startToken,
                        Identifier & ident,
                        Type & type,
                        AssignExpr & initExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
    virtual DataType & dataType() const final override;
    
    Type & mType;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
