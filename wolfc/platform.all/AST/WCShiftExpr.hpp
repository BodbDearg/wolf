#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;
class UnaryExpr;

/*
ShiftExpr:
	UnaryExpr << ShiftExpr
	UnaryExpr >> ShiftExpr
	UnaryExpr >>> ShiftExpr
*/
class ShiftExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ShiftExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* UnaryExpr */
class ShiftExprNoOp final : public ShiftExpr {
public:
    ShiftExprNoOp(UnaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    UnaryExpr & mExpr;
};

/* Base class for an ShiftExpr with two operands */
class ShiftExprTwoOps : public ShiftExpr {
public:
    ShiftExprTwoOps(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    UnaryExpr & mLeftExpr;
    ShiftExpr & mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of type 'int';
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
    
    /**
     * Do the actual codegen for the operation itself (runtime evaluation).
     * All other type checks have been done and sub-expressions evaluated at this point.
     */
    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) = 0;
    
    /**
     * Do the actual codegen for the operation itself (compile time evaluation).
     * All other type checks have been done and sub-expressions evaluated at this point.
     */
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) = 0;
};

/* UnaryExpr << ShiftExpr */
class ShiftExprLShift final : public ShiftExprTwoOps {
public:
    ShiftExprLShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);

    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) override;
};

/* UnaryExpr >> ShiftExpr */
class ShiftExprArithRShift final : public ShiftExprTwoOps {
public:
    ShiftExprArithRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);

    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) override;
};

/* UnaryExpr >>> ShiftExpr */
class ShiftExprLogicRShift final : public ShiftExprTwoOps {
public:
    ShiftExprLogicRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);

    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) override;
};

WC_END_NAMESPACE
