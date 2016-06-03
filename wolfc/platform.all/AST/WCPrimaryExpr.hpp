#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class BoolLit;
class Identifier;
class IntLit;
class ReadnumExpr;

/*
PrimaryExpr:
	IntLit
	BoolLit
    Identifier
    ReadnumExpr
*/
class PrimaryExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * currentToken);
    
    static PrimaryExpr * parse(const Token *& currentToken);
    
    /**
     * Tell if this expression evaluates to an lvalue. lvalues are values that can be asssigned to.
     * See lvalues versus rvalues: https://msdn.microsoft.com/en-us/library/f90831hc.aspx
     */
    virtual bool isLValue() const = 0;
    
    /* Codegen the llvm value that represents the address of this expression. Only possible for lvalues! */
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) = 0;
};

/* IntLit */
class PrimaryExprIntLit : public PrimaryExpr {
public:
    PrimaryExprIntLit(IntLit & lit);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    IntLit & mLit;
};

/* BoolLit */
class PrimaryExprBoolLit : public PrimaryExpr {
public:
    PrimaryExprBoolLit(BoolLit & lit);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    BoolLit & mLit;
};

/* Identifier */
class PrimaryExprIdentifier : public PrimaryExpr {
public:
    PrimaryExprIdentifier(Identifier & identifier);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    Identifier & mIdentifier;
};

/* ReadnumExpr */
class PrimaryExprReadnum : public PrimaryExpr {
public:
    PrimaryExprReadnum(ReadnumExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    ReadnumExpr & mExpr;
};

WC_END_NAMESPACE
