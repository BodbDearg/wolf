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
    BinaryExpr(ASTNode & parent);
    
    static bool peek(const Token * tokenPtr);
    
    static BinaryExpr * parse(ASTNode & parent, const Token *& tokenPtr);
};

/* 'UnaryExpression' */
class BinaryExprUnary : public BinaryExpr {
public:
    BinaryExprUnary(ASTNode & parent, UnaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    UnaryExpr & mExpr;
};

/* Base for a two operand binary expression */
class BinaryExprTwoOps : public BinaryExpr {
public:
    BinaryExprTwoOps(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    UnaryExpr & mLeftExpr;
    BinaryExpr & mRightExpr;
};

/* 'UnaryExpression + BinaryExpression' */
class BinaryExprAdd : public BinaryExprTwoOps {
public:
    BinaryExprAdd(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

/* 'UnaryExpression - BinaryExpression' */
class BinaryExprSub : public BinaryExprTwoOps {
public:
    BinaryExprSub(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

/* 'UnaryExpression * BinaryExpression' */
class BinaryExprMul : public BinaryExprTwoOps {
public:
    BinaryExprMul(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

/* 'UnaryExpression / BinaryExpression' */
class BinaryExprDiv : public BinaryExprTwoOps {
public:
    BinaryExprDiv(ASTNode & parent, UnaryExpr & leftExpr, BinaryExpr & rightExpr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
