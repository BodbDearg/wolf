#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class ArrayLit;
class BoolLit;
class DataType;
class Identifier;
class IntLit;
class LinearAlloc;
class RandExpr;
class ReadnumExpr;
class StrLit;
class TimeExpr;

/*
PrimaryExpr:
	IntLit
	BoolLit
	StrLit
    ArrayLit
    Identifier
    ReadnumExpr
    TimeExpr
    RandExpr
*/
class PrimaryExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * currentToken);
    static PrimaryExpr * parse(const Token *& currentToken, LinearAlloc & alloc);
};

/* IntLit */
class PrimaryExprIntLit final : public PrimaryExpr {
public:
    PrimaryExprIntLit(IntLit & lit);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    IntLit & mLit;
};

/* BoolLit */
class PrimaryExprBoolLit final : public PrimaryExpr {
public:
    PrimaryExprBoolLit(BoolLit & lit);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    BoolLit & mLit;
};

/* StrLit */
class PrimaryExprStrLit final : public PrimaryExpr {
public:
    PrimaryExprStrLit(StrLit & lit);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    StrLit & mLit;
};

/* ArrayLit */
class PrimaryExprArrayLit final : public PrimaryExpr {
public:
    PrimaryExprArrayLit(ArrayLit & lit);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    ArrayLit & mLit;
};

/* Identifier */
class PrimaryExprIdentifier final : public PrimaryExpr {
public:
    PrimaryExprIdentifier(Identifier & ident);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    const char * name() const;
    
    Identifier & mIdent;
};

/* ReadnumExpr */
class PrimaryExprReadnum final : public PrimaryExpr {
public:
    PrimaryExprReadnum(ReadnumExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    ReadnumExpr & mExpr;
};

/* TimeExpr */
class PrimaryExprTime final : public PrimaryExpr {
public:
    PrimaryExprTime(TimeExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    TimeExpr & mExpr;
};

/* RandExpr */
class PrimaryExprRandExpr final : public PrimaryExpr {
public:
    PrimaryExprRandExpr(RandExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    RandExpr & mExpr;
};

WC_END_NAMESPACE
