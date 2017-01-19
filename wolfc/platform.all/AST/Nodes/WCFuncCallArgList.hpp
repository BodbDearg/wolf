#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
FuncCallArgList:
	AssignExpr
	AssignExpr, FuncCallArgList
*/
class FuncCallArgList : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static FuncCallArgList * parse(ParseCtx & parseCtx);
    
    virtual size_t numArgs() const = 0;
    virtual void getArgs(std::vector<AssignExpr*> & args) const = 0;
};

/* AssignExpr */
class FuncCallArgListSingle final : public FuncCallArgList {
public:
    FuncCallArgListSingle(AssignExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    virtual void getArgs(std::vector<AssignExpr*> & args) const override;
    
    AssignExpr & mExpr;
};

/* AssignExpr , FuncCallArgList */
class FuncCallArgListMulti final : public FuncCallArgList {
public:
    FuncCallArgListMulti(AssignExpr & expr, FuncCallArgList & argList);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual size_t numArgs() const override;
    virtual void getArgs(std::vector<AssignExpr*> & args) const override;
    
    AssignExpr &        mExpr;
    FuncCallArgList &   mArgList;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
