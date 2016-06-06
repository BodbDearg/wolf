#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;

/*
PrintStmnt
    print ( AssignExpr )
*/
class PrintStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static PrintStmnt * parse(const Token *& tokenPtr);
    
    PrintStmnt(AssignExpr & expr, const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    AssignExpr & mExpr;
    const Token & mStartToken;
    const Token & mEndToken;
    
};

WC_END_NAMESPACE
