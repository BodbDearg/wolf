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
	PostfixExpr FuncCall
	PostfixExpr [ AssignExpr ]
*/
class PostfixExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static PostfixExpr * parse(const Token *& currentToken, LinearAlloc & alloc);
};

/* PrimaryExpr */
class PostfixExprNoPostfix final : public PostfixExpr {
public:
    PostfixExprNoPostfix(PrimaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    PrimaryExpr & mExpr;
};

/* PostfixExpr FuncCall */
class PostfixExprFuncCall final : public PostfixExpr {
public:
    PostfixExprFuncCall(PostfixExpr & expr, FuncCall & funcInvocation);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    PostfixExpr &   mExpr;
    FuncCall &      mFuncCall;
    
private:
    const char * nameOfFuncCalled() const;
    
    Func * lookupFuncCalled() const;
};

/* PostfixExpr [ AssignExpr ] */
class PostfixExprArrayLookup final : public PostfixExpr {
public:
    PostfixExprArrayLookup(PostfixExpr & arrayExpr,
                           AssignExpr & indexExpr,
                           const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    PostfixExpr &   mArrayExpr;
    AssignExpr &    mIndexExpr;
    const Token &   mEndToken;

private:
    /* Gets the array data type for the array expression. Issues a compile error on failure. */
    ArrayDataType * getArrayDataTypeOrIssueError();
    
    /* Peform codegen for getting the address of the array element */
    llvm::Value * codegenAddrOfArrayElem(CodegenCtx & cgCtx);
};

WC_END_NAMESPACE
