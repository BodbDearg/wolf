#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class DataType;
class PrimaryExpr;

/*
UnaryExpr:
	PrimaryExpr
	-PrimaryExpr
	+PrimaryExpr
	(AssignExpr)
*/
class UnaryExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * currentToken);
    
    static UnaryExpr * parse(const Token *& currentToken);
    
    /**
     * Tell if this expression evaluates to an lvalue. lvalues are values that can be asssigned to.
     * See lvalues versus rvalues: https://msdn.microsoft.com/en-us/library/f90831hc.aspx
     */
    virtual bool isLValue() const = 0;
    
    /* Return the data type of this expression */
    virtual const DataType & getDataType() const = 0;
    
    /* Codegen the llvm value that represents the address of this expression. Only possible for lvalues! */
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) = 0;
};

/* PrimaryExpr */
class UnaryExprPrimary : public UnaryExpr {
public:
    UnaryExprPrimary(PrimaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    PrimaryExpr & mExpr;
};

/* -PrimaryExpr */
class UnaryExprNegPrimary : public UnaryExpr {
public:
    UnaryExprNegPrimary(PrimaryExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    PrimaryExpr &   mExpr;    
    const Token &   mStartToken;
};

/* +PrimaryExpr */
class UnaryExprPosPrimary : public UnaryExprPrimary {
public:
    UnaryExprPosPrimary(PrimaryExpr & expr, const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    const Token &   mStartToken;
};

/* (AssignExpr) */
class UnaryExprParen : public UnaryExpr {
public:
    UnaryExprParen(AssignExpr & expr, const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    AssignExpr &    mExpr;
    const Token &   mStartToken;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
