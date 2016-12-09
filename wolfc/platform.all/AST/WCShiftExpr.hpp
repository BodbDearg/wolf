#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AddSubExpr;
class DataType;
class LinearAlloc;

/*
ShiftExpr:
	AddSubExpr << ShiftExpr
	AddSubExpr >> ShiftExpr
	AddSubExpr >>> ShiftExpr
*/
class ShiftExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ShiftExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* AddSubExpr */
class ShiftExprNoOp final : public ShiftExpr {
public:
    ShiftExprNoOp(AddSubExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    AddSubExpr & mExpr;
};

/* Base class for an ShiftExpr with two operands */
class ShiftExprTwoOps : public ShiftExpr {
public:
    ShiftExprTwoOps(AddSubExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    
    AddSubExpr &    mLeftExpr;
    ShiftExpr &     mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of type 'int';
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

/* AddSubExpr << ShiftExpr */
class ShiftExprLShift final : public ShiftExprTwoOps {
public:
    ShiftExprLShift(AddSubExpr & leftExpr, ShiftExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr >> ShiftExpr */
class ShiftExprArithRShift final : public ShiftExprTwoOps {
public:
    ShiftExprArithRShift(AddSubExpr & leftExpr, ShiftExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

/* AddSubExpr >>> ShiftExpr */
class ShiftExprLogicRShift final : public ShiftExprTwoOps {
public:
    ShiftExprLogicRShift(AddSubExpr & leftExpr, ShiftExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
