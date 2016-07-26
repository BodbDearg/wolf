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
class AddSubExprNoOp : public AddSubExpr {
public:
    AddSubExprNoOp(MulDivExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    MulDivExpr & mExpr;
};

/* MulDivExpr + AddSubExpr */
class AddSubExprAdd : public AddSubExpr {
public:
    AddSubExprAdd(MulDivExpr & leftExpr, AddSubExpr & rightExpr);
    
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
    
    MulDivExpr & mLeftExpr;
    AddSubExpr & mRightExpr;
};

/* MulDivExpr - AddSubExpr */
class AddSubExprSub : public AddSubExpr {
public:
    AddSubExprSub(MulDivExpr & leftExpr, AddSubExpr & rightExpr);
    
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
    
    MulDivExpr & mLeftExpr;
    AddSubExpr & mRightExpr;
};

WC_END_NAMESPACE
