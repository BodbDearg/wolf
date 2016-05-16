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
    
    static UnaryExpr * parse(ASTNode & parent, const Token *& currentToken);
    
    UnaryExpr(ASTNode & parent);
};

/* IntLit */
class UnaryExprIntLit : public UnaryExpr {
public:
    UnaryExprIntLit(ASTNode & parent, IntLit & lit);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    IntLit & mLit;
};

/* -IntLit */
class UnaryExprIntLitNeg : public UnaryExpr {
public:
    UnaryExprIntLitNeg(ASTNode & parent, IntLit & lit);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    IntLit & mLit;
};

/* +IntLit */
class UnaryExprIntLitPos : public UnaryExprIntLit {
public:
    UnaryExprIntLitPos(ASTNode & parent, IntLit & lit);
};

/* (BinaryExpr) */
class UnaryExprParen : public UnaryExpr {
public:
    UnaryExprParen(ASTNode & parent, BinaryExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mExpr;
};

WC_END_NAMESPACE
