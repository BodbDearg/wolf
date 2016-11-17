#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class MulDivExpr;

/*
AddSubExpr:
	MulDivExpr
	MulDivExpr + AddSubExpr
	MulDivExpr - AddSubExpr
*/
class AddSubExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static AddSubExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* MulDivExpr */
class AddSubExprNoOp final : public AddSubExpr {
public:
    AddSubExprNoOp(MulDivExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual bool requiresStorage() const override;
    virtual llvm::Value * getStorage() const override;
    virtual void setStorage(llvm::Value & storage) override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    MulDivExpr & mExpr;
};

/* Base class for an AddSubExpr with two operands */
class AddSubExprTwoOps : public AddSubExpr {
public:
    AddSubExprTwoOps(MulDivExpr & leftExpr, AddSubExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    MulDivExpr & mLeftExpr;
    AddSubExpr & mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* MulDivExpr + AddSubExpr */
class AddSubExprAdd final : public AddSubExprTwoOps {
public:
    AddSubExprAdd(MulDivExpr & leftExpr, AddSubExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* MulDivExpr - AddSubExpr */
class AddSubExprSub final : public AddSubExprTwoOps {
public:
    AddSubExprSub(MulDivExpr & leftExpr, AddSubExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
