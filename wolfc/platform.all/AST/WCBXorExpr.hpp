#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class BAndExpr;
class DataType;
class LinearAlloc;

/*
BXorExpr:
	BAndExpr
	BAndExpr ^ BXorExpr
*/
class BXorExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static BXorExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* BAndExpr */
class BXorExprNoOp final : public BXorExpr {
public:
    BXorExprNoOp(BAndExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    BAndExpr & mExpr;
};

/* BAndExpr ^ BXorExpr */
class BXorExprXor final : public BXorExpr {
public:
    BXorExprXor(BAndExpr & leftExpr, BXorExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    BAndExpr & mLeftExpr;
    BXorExpr & mRightExpr;
    
protected:
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of type 'int';
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
};

WC_END_NAMESPACE
