#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class DataType;
class Identifier;
class LinearAlloc;
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
    
    bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode);
    
    Identifier &    mIdent;
    Type &          mType;
};

WC_END_NAMESPACE
