#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class ArrayDataType;
class AssignExpr;
class DataType;
class Func;
class FuncCall;
class LinearAlloc;
class PrimaryExpr;

/*
PostfixExpr:
	PrimaryExpr
	PrimaryExpr FuncCall
    PrimaryExpr [ AssignExpr ]
*/
class PostfixExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    
    static PostfixExpr * parse(const Token *& currentToken, LinearAlloc & alloc);
};

/* PrimaryExpr */
class PostfixExprNoPostfix : public PostfixExpr {
public:
    PostfixExprNoPostfix(PrimaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool requiresStorage() const override;
    
    virtual llvm::Value * getStorage() const override;
    
    virtual void setStorage(llvm::Value & storage) override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    PrimaryExpr & mExpr;
};

/* PrimaryExpr FuncCall */
class PostfixExprFuncCall : public PostfixExpr {
public:
    PostfixExprFuncCall(PrimaryExpr & expr, FuncCall & funcInvocation);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    const char * nameOfFuncCalled() const;
    
    Func * lookupFuncCalled() const;
    
    PrimaryExpr &   mExpr;
    FuncCall &      mFuncCall;
};

/* PrimaryExpr [ AssignExpr ] */
class PostfixExprArrayLookup : public PostfixExpr {
public:
    PostfixExprArrayLookup(PrimaryExpr & arrayExpr,
                           AssignExpr & indexExpr,
                           const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    /* Gets the array data type for the array expression. Issues a compile error on failure. */
    ArrayDataType * getArrayDataTypeOrIssueError();
    
    /* Peform codegen for getting the address of the array element */
    llvm::Value * codegenAddrOfArrayElem(CodegenCtx & cgCtx);
    
    PrimaryExpr &   mArrayExpr;
    AssignExpr &    mIndexExpr;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
