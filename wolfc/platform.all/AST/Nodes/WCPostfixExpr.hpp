#pragma once

#include "DataType/WCDataTypeCodegenFuncs.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class ArrayDataType;

WC_AST_BEGIN_NAMESPACE

class AssignExpr;
class CastExpr;
class Func;
class FuncCall;

/*
PostfixExpr:
	CastExpr
	CastExpr ++
	CastExpr --
	PostfixExpr FuncCall
	PostfixExpr [ AssignExpr ]
*/
class PostfixExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static PostfixExpr * parse(ParseCtx & parseCtx);
};

/* CastExpr */
class PostfixExprNoPostfix final : public PostfixExpr {
public:
    PostfixExprNoPostfix(CastExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    CastExpr & mExpr;
};

/* Base class for increment/decrement postfix expressions */
class PostfixExprIncDecBase : public PostfixExpr {
public:
    PostfixExprIncDecBase(CastExpr & expr,
                          const Token & endToken
                        #warning FIXME - Codegen
                        #if 0
                          ,DTCodegenUnaryOpFunc codegenUnaryOpFunc
                        #endif
                          );

    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;

    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;

    virtual DataType & dataType() final override;

#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) final override;
#endif
    
    CastExpr &      mExpr;
    const Token &   mEndToken;

private:
#warning FIXME - Codegen
#if 0
    const DTCodegenUnaryOpFunc mCodegenUnaryOpFunc;
#endif
};

/* CastExpr ++ */
class PostfixExprInc final : public PostfixExprIncDecBase {
public:
    PostfixExprInc(CastExpr & expr, const Token & endToken);
};

/* CastExpr -- */
class PostfixExprDec final : public PostfixExprIncDecBase {
public:
    PostfixExprDec(CastExpr & expr, const Token & endToken);
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
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    PostfixExpr &   mExpr;
    FuncCall &      mFuncCall;
    
private:
    const char * nameOfFuncCalled() const;
    
    Func * lookupFuncCalled() const;
    
#warning FIXME - Codegen
#if 0
    llvm::Value * mAddrOfResult;
    llvm::Value * mExprEvalResult;
#endif
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
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    PostfixExpr &   mArrayExpr;
    AssignExpr &    mIndexExpr;
    const Token &   mEndToken;

private:
    /* Gets the array data type for the array expression. Issues a compile error on failure. */
    ArrayDataType * getArrayDataTypeOrIssueError();
    
#warning FIXME - Codegen
#if 0
    /* Peform codegen for getting the address of the array element */
    llvm::Value * codegenAddrOfArrayElem(CodegenCtx & cgCtx);
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
