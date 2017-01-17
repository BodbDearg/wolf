#pragma once

#include "WCASTNode.hpp"

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
    
    FuncArg(Type & dataType, Identifier & ident);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    DataType & dataType();
    
    const char * name() const;
    
#warning FIXME - Codegen
#if 0
    bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode);
#endif
    
    Identifier &    mIdent;
    Type &          mType;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
