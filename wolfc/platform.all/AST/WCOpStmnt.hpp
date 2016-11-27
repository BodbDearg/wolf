#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class TernaryExpr;

/*
OpStmnt:
    TernaryExpr
    TernaryExpr = TernaryExpr
*/
class OpStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static OpStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* TernaryExpr */
class OpStmntExpr final : public OpStmnt {
public:
    OpStmntExpr(TernaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    TernaryExpr & mExpr;
};

/* TernaryExpr = TernaryExpr */
class OpStmntAssign final : public OpStmnt {
public:
    OpStmntAssign(TernaryExpr & leftExpr, TernaryExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;

    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    TernaryExpr & mLeftExpr;
    TernaryExpr & mRightExpr;
};

WC_END_NAMESPACE
