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
};

/* 'UnaryExpression' */
class BinaryExprUnary : public BinaryExpr {
public:
    BinaryExprUnary(UnaryExpr & expr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    UnaryExpr & mExpr;
};

/* Base for a two operand binary expression */
class BinaryExprTwoOps : public BinaryExpr {
public:
    BinaryExprTwoOps(UnaryExpr & leftExpr,
                     BinaryExpr & rightExpr);
    
    UnaryExpr & mLeftExpr;
    BinaryExpr & mRightExpr;
};

/* 'UnaryExpression + BinaryExpression' */
class BinaryExprAdd : public BinaryExprTwoOps {
public:
    BinaryExprAdd(UnaryExpr & leftExpr,
                  BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* 'UnaryExpression - BinaryExpression' */
class BinaryExprSub : public BinaryExprTwoOps {
public:
    BinaryExprSub(UnaryExpr & leftExpr,
                  BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* 'UnaryExpression * BinaryExpression' */
class BinaryExprMul : public BinaryExprTwoOps {
public:
    BinaryExprMul(UnaryExpr & leftExpr,
                  BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* 'UnaryExpression / BinaryExpression' */
class BinaryExprDiv : public BinaryExprTwoOps {
public:
    BinaryExprDiv(UnaryExpr & leftExpr,
                  BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

WC_END_NAMESPACE