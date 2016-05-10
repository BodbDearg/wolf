#pragma once

#include "WCASTNode.hpp"

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
class BinaryExpr : public ASTNode {
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
                     UnaryExpr & rightExpr);
    
    UnaryExpr & mLeftExpr;
    UnaryExpr & mRightExpr;
};

/* 'UnaryExpression + BinaryExpression' */
class BinaryExprAdd : public BinaryExprTwoOps {
public:
    BinaryExprAdd(UnaryExpr & leftExpr,
                  UnaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* 'UnaryExpression - BinaryExpression' */
class BinaryExprSub : public BinaryExprTwoOps {
public:
    BinaryExprSub(UnaryExpr & leftExpr,
                  UnaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* 'UnaryExpression * BinaryExpression' */
class BinaryExprMul : public BinaryExprTwoOps {
public:
    BinaryExprMul(UnaryExpr & leftExpr,
                  UnaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* 'UnaryExpression / BinaryExpression' */
class BinaryExprDiv : public BinaryExprTwoOps {
public:
    BinaryExprDiv(UnaryExpr & leftExpr,
                  UnaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

WC_END_NAMESPACE
