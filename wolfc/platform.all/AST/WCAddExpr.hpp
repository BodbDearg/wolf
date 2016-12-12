#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class MulExpr;

/*
AddExpr:
	MulExpr
	MulExpr + AddExpr
	MulExpr - AddExpr
*/
class AddExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static AddExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* MulExpr */
class AddExprNoOp final : public AddExpr {
public:
    AddExprNoOp(MulExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    MulExpr & mExpr;
};

/* Base class for an AddExpr with two operands */
class AddExprTwoOps : public AddExpr {
public:
    AddExprTwoOps(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    MulExpr &   mLeftExpr;
    AddExpr &   mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of type 'int';
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* MulExpr + AddExpr */
class AddExprAdd final : public AddExprTwoOps {
public:
    AddExprAdd(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* MulExpr - AddExpr */
class AddExprSub final : public AddExprTwoOps {
public:
    AddExprSub(MulExpr & leftExpr, AddExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
