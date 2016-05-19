#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class BinaryExpr;

/*
AssignExpr:
    BinaryExpr
    BinaryExpr = AssignExpr
*/
class AssignExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static AssignExpr * parse(const Token *& tokenPtr);
    
    /**
     * Tell if this expression evaluates to an lvalue. lvalues are values that can be asssigned to.
     * See lvalues versus rvalues: https://msdn.microsoft.com/en-us/library/f90831hc.aspx
     */
    virtual bool isLValue() const = 0;
    
    /* Codegen the llvm value that represents the address of this expression. Only possible for lvalues! */
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) = 0;
};

/* BinaryExpr */
class AssignExprNoAssign : public AssignExpr {
public:
    AssignExprNoAssign(BinaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mExpr;
};

/* BinaryExpr = AssignExpr */
class AssignExprAssign : public AssignExpr {
public:
    AssignExprAssign(BinaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    virtual bool isLValue() const override;

    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    BinaryExpr & mLeftExpr;
    AssignExpr & mRightExpr;
};

WC_END_NAMESPACE
