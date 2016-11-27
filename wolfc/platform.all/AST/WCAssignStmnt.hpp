#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class TernaryExpr;

/*
AssignStmnt:
    TernaryExpr
    TernaryExpr = TernaryExpr
*/
class AssignStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static AssignStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* TernaryExpr */
class AssignStmntNoAssign final : public AssignStmnt {
public:
    AssignStmntNoAssign(TernaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    TernaryExpr & mExpr;
};

/* TernaryExpr = TernaryExpr */
class AssignStmntAssign final : public AssignStmnt {
public:
    AssignStmntAssign(TernaryExpr & leftExpr, TernaryExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    TernaryExpr & mLeftExpr;
    TernaryExpr & mRightExpr;
};

WC_END_NAMESPACE
