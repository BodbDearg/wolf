#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class UnaryExpr;

/*
BinaryExpr:
	UnaryExpr
	UnaryExpr + BinaryExpr
	UnaryExpr - BinaryExpr
	UnaryExpr / BinaryExpr
	UnaryExpr * BinaryExpr
*/
class BinaryExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static BinaryExpr * parse(const Token *& tokenPtr);
    
    /**
     * Tell if this expression evaluates to an lvalue. lvalues are values that can be asssigned to.
     * See lvalues versus rvalues: https://msdn.microsoft.com/en-us/library/f90831hc.aspx
     */
    virtual bool isLValue() const = 0;
    
    /* Codegen the llvm value that represents the address of this expression. Only possible for lvalues! */
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) = 0;
};

/* 'UnaryExpression' */
class BinaryExprUnary : public BinaryExpr {
public:
    BinaryExprUnary(UnaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    UnaryExpr & mExpr;
};

/* Base for a two operand binary expression */
class BinaryExprTwoOps : public BinaryExpr {
public:
    BinaryExprTwoOps(UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual bool isLValue() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    UnaryExpr & mLeftExpr;
    BinaryExpr & mRightExpr;
};

/* 'UnaryExpression + BinaryExpression' */
class BinaryExprAdd : public BinaryExprTwoOps {
public:
    BinaryExprAdd(UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

/* 'UnaryExpression - BinaryExpression' */
class BinaryExprSub : public BinaryExprTwoOps {
public:
    BinaryExprSub(UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

/* 'UnaryExpression * BinaryExpression' */
class BinaryExprMul : public BinaryExprTwoOps {
public:
    BinaryExprMul(UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

/* 'UnaryExpression / BinaryExpression' */
class BinaryExprDiv : public BinaryExprTwoOps {
public:
    BinaryExprDiv(UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
