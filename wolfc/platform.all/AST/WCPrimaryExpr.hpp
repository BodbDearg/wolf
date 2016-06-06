#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class BoolLit;
class DataType;
class Identifier;
class IntLit;
class ReadnumExpr;
class StrLit;

/*
PrimaryExpr:
	IntLit
	BoolLit
	StrLit
    Identifier
    ReadnumExpr
*/
class PrimaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    
    static PrimaryExpr * parse(const Token *& currentToken);
};

/* IntLit */
class PrimaryExprIntLit : public PrimaryExpr {
public:
    PrimaryExprIntLit(IntLit & lit);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    IntLit & mLit;
};

/* BoolLit */
class PrimaryExprBoolLit : public PrimaryExpr {
public:
    PrimaryExprBoolLit(BoolLit & lit);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    BoolLit & mLit;
};

/* StrLit */
class PrimaryExprStrLit : public PrimaryExpr {
public:
    PrimaryExprStrLit(StrLit & lit);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    StrLit & mLit;
};

/* Identifier */
class PrimaryExprIdentifier : public PrimaryExpr {
public:
    PrimaryExprIdentifier(Identifier & ident);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    Identifier & mIdent;
};

/* ReadnumExpr */
class PrimaryExprReadnum : public PrimaryExpr {
public:
    PrimaryExprReadnum(ReadnumExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    ReadnumExpr & mExpr;
};

WC_END_NAMESPACE
