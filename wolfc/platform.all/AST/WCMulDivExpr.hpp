#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class UnaryExpr;

/*
MulDivExpr:
	UnaryExpr
	UnaryExpr * MulDivExpr
	UnaryExpr / MulDivExpr
*/
class MulDivExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static MulDivExpr * parse(const Token *& tokenPtr);
    
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

/* UnaryExpression */
class MulDivExprNoOp : public MulDivExpr {
public:
    MulDivExprNoOp(UnaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    UnaryExpr & mExpr;
};

/* UnaryExpr * MulDivExpr */
class MulDivExprMul : public MulDivExpr {
public:
    MulDivExprMul(UnaryExpr & leftExpr, MulDivExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    UnaryExpr & mLeftExpr;
    MulDivExpr & mRightExpr;
};

/* UnaryExpr / MulDivExpr */
class MulDivExprDiv : public MulDivExpr {
public:
    MulDivExprDiv(UnaryExpr & leftExpr, MulDivExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & getDataType() const override;
    
    /**
     * TODO: this is a temp function for the moment. Issue a compile error either the left or right expr is not of 'int'
     * return false for failure if that is the case.
     */
    bool compileCheckBothExprsAreInt() const;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    UnaryExpr & mLeftExpr;
    MulDivExpr & mRightExpr;
};

WC_END_NAMESPACE
