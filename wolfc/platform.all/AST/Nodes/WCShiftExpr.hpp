#pragma once

#warning FIXME - Codegen
#if 0
#include "DataType/WCDataTypeCodegenFuncs.hpp"
#endif

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class UnaryExpr;

/*
ShiftExpr:
	UnaryExpr << ShiftExpr
	UnaryExpr >> ShiftExpr
	UnaryExpr >>> ShiftExpr
*/
class ShiftExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ShiftExpr * parse(ParseCtx & parseCtx);
};

/* UnaryExpr */
class ShiftExprNoOp final : public ShiftExpr {
public:
    ShiftExprNoOp(UnaryExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
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
    
    UnaryExpr & mExpr;
};

/* Base class for an ShiftExpr with two operands */
class ShiftExprTwoOps : public ShiftExpr {
public:
    ShiftExprTwoOps(UnaryExpr & leftExpr,
                    ShiftExpr & rightExpr
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
    
    UnaryExpr & mLeftExpr;
    ShiftExpr & mRightExpr;
    
#warning FIXME - Codegen
#if 0
private:
    const DTCodegenBinaryOpFunc         mCodegenBinaryOpFunc;
    const DTCodegenConstBinaryOpFunc    mCodegenConstBinaryOpFunc;
#endif
};

/* UnaryExpr << ShiftExpr */
class ShiftExprLShift final : public ShiftExprTwoOps {
public:
    ShiftExprLShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* UnaryExpr >> ShiftExpr */
class ShiftExprARShift final : public ShiftExprTwoOps {
public:
    ShiftExprARShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* UnaryExpr >>> ShiftExpr */
class ShiftExprLRShift final : public ShiftExprTwoOps {
public:
    ShiftExprLRShift(UnaryExpr & leftExpr, ShiftExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
