#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class TernaryExpr;

/*
AssignExpr:
    TernaryExpr
    TernaryExpr = AssignExpr
*/
class AssignExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static AssignExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* TernaryExpr */
class AssignExprNoAssign final : public AssignExpr {
public:
    AssignExprNoAssign(TernaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * getStorage() const override;
    virtual void setStorage(llvm::Value & storage) override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    TernaryExpr & mExpr;
};

/* TernaryExpr = AssignExpr */
class AssignExprAssign final : public AssignExpr {
public:
    AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    TernaryExpr &   mLeftExpr;
    AssignExpr &    mRightExpr;
};

WC_END_NAMESPACE
