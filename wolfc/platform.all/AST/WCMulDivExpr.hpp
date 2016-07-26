#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class UnaryExpr;

/*
MulDivExpr:
	UnaryExpr
	UnaryExpr * MulDivExpr
	UnaryExpr / MulDivExpr
*/
class MulDivExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static MulDivExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* UnaryExpression */
class MulDivExprNoOp : public MulDivExpr {
public:
    MulDivExprNoOp(UnaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    UnaryExpr & mExpr;
};

/* UnaryExpr * MulDivExpr */
class MulDivExprMul : public MulDivExpr {
public:
    MulDivExprMul(UnaryExpr & leftExpr, MulDivExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;    
    
    UnaryExpr & mLeftExpr;
    MulDivExpr & mRightExpr;
};

/* UnaryExpr / MulDivExpr */
class MulDivExprDiv : public MulDivExpr {
public:
    MulDivExprDiv(UnaryExpr & leftExpr, MulDivExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;    
    
    UnaryExpr & mLeftExpr;
    MulDivExpr & mRightExpr;
};

WC_END_NAMESPACE
