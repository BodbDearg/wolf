#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;
class TernaryExpr;

/*
PrintStmnt:
    print ( TernaryExpr )
*/
class PrintStmnt final : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static PrintStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    PrintStmnt(TernaryExpr & expr, const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    TernaryExpr & mExpr;
    const Token & mStartToken;
    const Token & mEndToken;
};

WC_END_NAMESPACE
