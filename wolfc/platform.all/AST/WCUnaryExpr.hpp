#pragma once

#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

class BinaryExpr;
class UIntLit;

/*
UnaryExpr:
	UIntLit
	-UIntLit
	+UIntLit
	(BinaryExpr)
*/
class UnaryExpr : public ASTNode {
public:
    static bool peek(const Token * currentToken);
    
    static UnaryExpr * parse(const Token *& currentToken);
};

/* UIntLit */
class UnaryExprUIntLit : public UnaryExpr {
public:
    UnaryExprUIntLit(UIntLit & lit);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* -UIntLit */
class UnaryExprUIntLitNeg : public UnaryExpr {
public:
    UnaryExprUIntLitNeg(UIntLit & lit);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
};

/* +UIntLit */
class UnaryExprUIntLitPos : public UnaryExprUIntLit {
public:
    UnaryExprUIntLitPos(UIntLit & lit);
};

/* (BinaryExpr) */
class UnaryExprParen : public UnaryExprUIntLit {
public:
    UnaryExprParen(BinaryExpr & lit);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
