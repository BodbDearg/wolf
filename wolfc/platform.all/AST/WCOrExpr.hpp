#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class AndExpr;
class DataType;

/*
OrExpr:
	AndExpr
	AndExpr or OrExpr
*/
class OrExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static OrExpr * parse(const Token *& tokenPtr);
    
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

/* AndExpr */
class OrExprNoOp : public OrExpr {
public:
    OrExprNoOp(AndExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    AndExpr & mExpr;
};

/* AndExpr or OrExpr */
class OrExprOr : public OrExpr {
public:
    OrExprOr(AndExpr & leftExpr, OrExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;

    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    AndExpr & mLeftExpr;
    OrExpr & mRightExpr;
};

WC_END_NAMESPACE
