#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class BinaryExpr;
class IntLit;

/*
UnaryExpr:
	IntLit
	-IntLit
	+IntLit
	(BinaryExpr)
*/
class UnaryExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * currentToken);
    
    static UnaryExpr * parse(const Token *& currentToken);
};

/* IntLit */
class UnaryExprIntLit : public UnaryExpr {
public:
    UnaryExprIntLit(IntLit & lit);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    IntLit & mLit;
};

/* -IntLit */
class UnaryExprIntLitNeg : public UnaryExpr {
public:
    UnaryExprIntLitNeg(IntLit & lit);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    IntLit & mLit;
};

/* +IntLit */
class UnaryExprIntLitPos : public UnaryExprIntLit {
public:
    UnaryExprIntLitPos(IntLit & lit);
};

/* (BinaryExpr) */
class UnaryExprParen : public UnaryExpr {
public:
    UnaryExprParen(BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
