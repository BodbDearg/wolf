#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class AssignExpr;

/*
IfStmnt:
	if AssignExpr then Scope end
	if AssignExpr then Scope else IfStmnt
	if AssignExpr then Scope else Scope end
*/
class IfStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static IfStmnt * parse(const Token *& tokenPtr);
};

/* if AssignExpr then Scope end */
class IfStmntNoElse : public IfStmnt {
public:
    IfStmntNoElse(AssignExpr & ifExpr,
                  Scope & innerScope,
                  const Token & startToken,
                  const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    AssignExpr &    mIfExpr;
    Scope &         mInnerScope;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
