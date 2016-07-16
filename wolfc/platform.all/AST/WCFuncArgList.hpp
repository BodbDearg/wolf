#pragma once

#include "WCASTNode.hpp"
#include <vector>

WC_BEGIN_NAMESPACE

class FuncArg;
class LinearAlloc;

/*
FuncArgList:
	FuncArg
	FuncArg , FuncArgList
*/
class FuncArgList : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static FuncArgList * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    virtual size_t numArgs() const = 0;
    
    virtual void getArgs(std::vector<FuncArg*> & args) = 0;
};

/* FuncArg */
class FuncArgListSingle : public FuncArgList {
public:
    FuncArgListSingle(FuncArg & arg);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    
    virtual void getArgs(std::vector<FuncArg*> & args) override;
    
    FuncArg & mArg;
};

/* FuncArg , FuncArgList */
class FuncArgListMulti : public FuncArgList {
public:
    FuncArgListMulti(FuncArg & arg, FuncArgList & argList);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    
    virtual void getArgs(std::vector<FuncArg*> & args) override;
    
    FuncArg &       mArg;
    FuncArgList &   mArgList;
};

WC_END_NAMESPACE