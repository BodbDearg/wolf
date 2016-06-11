#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class OrExpr;

/*
AssignExpr:
    OrExpr
    OrExpr = AssignExpr
*/
class AssignExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static AssignExpr * parse(const Token *& tokenPtr);
};

/* OrExpr */
class AssignExprNoAssign : public AssignExpr {
public:
    AssignExprNoAssign(OrExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    OrExpr & mExpr;
};

/* OrExpr = AssignExpr */
class AssignExprAssign : public AssignExpr {
public:
    AssignExprAssign(OrExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    OrExpr & mLeftExpr;
    AssignExpr & mRightExpr;
};

WC_END_NAMESPACE
