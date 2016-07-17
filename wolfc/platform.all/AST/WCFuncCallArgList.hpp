#pragma once

#include "WCASTNode.hpp"
#include <vector>

WC_BEGIN_NAMESPACE

class AssignExpr;
class LinearAlloc;

/*
FuncCallArgList:
	AssignExpr
	AssignExpr, FuncCallArgList
*/
class FuncCallArgList : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static FuncCallArgList * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    virtual size_t numArgs() const = 0;
    
    virtual void getArgs(std::vector<AssignExpr*> & args) const = 0;
};

/* AssignExpr */
class FuncCallArgListSingle : public FuncCallArgList {
public:
    FuncCallArgListSingle(AssignExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    
    virtual void getArgs(std::vector<AssignExpr*> & args) const override;
    
    AssignExpr & mExpr;
};

/* AssignExpr , FuncCallArgList */
class FuncCallArgListMulti : public FuncCallArgList {
public:
    FuncCallArgListMulti(AssignExpr & expr, FuncCallArgList & argList);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    
    virtual void getArgs(std::vector<AssignExpr*> & args) const override;
    
    AssignExpr &        mExpr;
    FuncCallArgList &   mArgList;
};

WC_END_NAMESPACE
