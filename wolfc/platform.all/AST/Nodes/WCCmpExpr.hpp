#pragma once

#warning FIXME - Codegen
#if 0
#include "DataType/WCDataTypeCodegenFuncs.hpp"
#endif

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AddExpr;

/*
CmpExpr:
	AddExpr
	AddExpr == CmpExpr
	AddExpr is CmpExpr
	AddExpr != CmpExpr
	AddExpr is not CmpExpr
	AddExpr < CmpExpr
	AddExpr <= CmpExpr
	AddExpr > CmpExpr
	AddExpr >= CmpExpr
*/
class CmpExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static CmpExpr * parse(ParseCtx & parseCtx);
};

/* AddExpr */
class CmpExprNoOp final : public CmpExpr {
public:
    CmpExprNoOp(AddExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    AddExpr & mExpr;
};

/* Base for relational expressions with two operators. */
class CmpExprTwoOps : public CmpExpr {
public:
    CmpExprTwoOps(AddExpr & leftExpr,
                  CmpExpr & rightExpr
                #warning FIXME - Codegen
                #if 0
                  ,DTCodegenBinaryOpFunc codegenBinaryOpFunc,
                  DTCodegenConstBinaryOpFunc codegenConstBinaryOpFunc
                #endif
                  );
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual DataType & dataType() final override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    AddExpr &   mLeftExpr;
    CmpExpr &   mRightExpr;
    
#warning FIXME - Codegen
#if 0
private:
    const DTCodegenBinaryOpFunc         mCodegenBinaryOpFunc;
    const DTCodegenConstBinaryOpFunc    mCodegenConstBinaryOpFunc;
#endif
};

/**
 * AddExpr == CmpExpr
 * AddExpr is CmpExpr
 */
class CmpExprEQ final : public CmpExprTwoOps {
public:
    CmpExprEQ(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

/** 
 * AddExpr != CmpExpr
 * AddExpr is not CmpExpr
 */
class CmpExprNE final : public CmpExprTwoOps {
public:
    CmpExprNE(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

/* AddExpr < CmpExpr */
class CmpExprLT final : public CmpExprTwoOps {
public:
    CmpExprLT(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

/* AddExpr <= CmpExpr */
class CmpExprLE final : public CmpExprTwoOps {
public:
    CmpExprLE(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

/* AddExpr > CmpExpr */
class CmpExprGT final : public CmpExprTwoOps {
public:
    CmpExprGT(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

/* AddExpr >= CmpExpr */
class CmpExprGE final : public CmpExprTwoOps {
public:
    CmpExprGE(AddExpr & leftExpr, CmpExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
