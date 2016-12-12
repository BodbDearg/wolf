#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class ShiftExpr;

/*
MulExpr:
	ShiftExpr
	ShiftExpr * MulExpr
	ShiftExpr / MulExpr
	ShiftExpr % MulExpr
*/
class MulExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static MulExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* ShiftExpr */
class MulExprNoOp final : public MulExpr {
public:
    MulExprNoOp(ShiftExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    ShiftExpr & mExpr;
};

/* Base class for an MulExpr with two operands */
class MulExprTwoOps : public MulExpr {
public:
    MulExprTwoOps(ShiftExpr & leftExpr, MulExpr & rightExpr);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    ShiftExpr &     mLeftExpr;
    MulExpr &       mRightExpr;
    
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

/* ShiftExpr * MulExpr */
class MulExprMul final : public MulExprTwoOps {
public:
    MulExprMul(ShiftExpr & leftExpr, MulExpr & rightExpr);

    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) override;
};

/* ShiftExpr / MulExpr */
class MulExprDiv final : public MulExprTwoOps {
public:
    MulExprDiv(ShiftExpr & leftExpr, MulExpr & rightExpr);

    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) override;
};

/* ShiftExpr % MulExpr */
class MulExprMod final : public MulExprTwoOps {
public:
    MulExprMod(ShiftExpr & leftExpr, MulExpr & rightExpr);

    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) override;
};

/* ShiftExpr & MulExpr */
class MulExprBAnd final : public MulExprTwoOps {
public:
    MulExprBAnd(ShiftExpr & leftExpr, MulExpr & rightExpr);
    
    virtual llvm::Value * codegenOpEval(CodegenCtx & cgCtx,
                                        llvm::Value & leftVal,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenOpConstEval(llvm::Constant & leftVal,
                                                llvm::Constant & rightVal) override;
};


WC_END_NAMESPACE
