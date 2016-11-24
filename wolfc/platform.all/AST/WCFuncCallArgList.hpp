#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class LinearAlloc;
class TernaryExpr;

/*
FuncCallArgList:
	TernaryExpr
	TernaryExpr, FuncCallArgList
*/
class FuncCallArgList : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static FuncCallArgList * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    virtual size_t numArgs() const = 0;
    virtual void getArgs(std::vector<TernaryExpr*> & args) const = 0;
};

/* TernaryExpr */
class FuncCallArgListSingle final : public FuncCallArgList {
public:
    FuncCallArgListSingle(TernaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    virtual void getArgs(std::vector<TernaryExpr*> & args) const override;
    
    TernaryExpr & mExpr;
};

/* TernaryExpr , FuncCallArgList */
class FuncCallArgListMulti final : public FuncCallArgList {
public:
    FuncCallArgListMulti(TernaryExpr & expr, FuncCallArgList & argList);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    virtual void getArgs(std::vector<TernaryExpr*> & args) const override;
    
    TernaryExpr &       mExpr;
    FuncCallArgList &   mArgList;
};

WC_END_NAMESPACE
