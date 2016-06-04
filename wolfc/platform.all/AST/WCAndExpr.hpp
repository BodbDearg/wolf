#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class NotExpr;

/*
AndExpr:
	NotExpr
	NotExpr and AndExpr
*/
class AndExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static AndExpr * parse(const Token *& tokenPtr);
    
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

/* NotExpr */
class AndExprNoOp : public AndExpr {
public:
    AndExprNoOp(NotExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    NotExpr & mExpr;
};

/* NotExpr and AndExpr */
class AndExprAnd : public AndExpr {
public:
    AndExprAnd(NotExpr & leftExpr, AndExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;

    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    NotExpr & mLeftExpr;
    AndExpr & mRightExpr;
};

WC_END_NAMESPACE
