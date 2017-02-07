#pragma once

#include "ASTNode.hpp"

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class Identifier;
class Type;

/*
FuncArg:
	Identifier : Type
*/
class FuncArg final : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    static FuncArg * parse(ParseCtx & parseCtx);
    
    FuncArg(Type & type, Identifier & ident);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const DataType & dataType() const;
    
    const char * name() const;
    
    Identifier &    mIdent;
    Type &          mType;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
