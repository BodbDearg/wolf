#pragma once

#include "DataType/WCDataTypeCodegenFuncs.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class MulExpr;

/*
AddExpr:
	MulExpr
	MulExpr + AddExpr
	MulExpr - AddExpr
	MulExpr | AddExpr
	MulExpr ^ AddExpr
*/
class AddExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static AddExpr * parse(ParseCtx & parseCtx);
};

/* MulExpr */
class AddExprNoOp final : public AddExpr {
public:
    AddExprNoOp(MulExpr & expr);
    
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
    
    MulExpr & mExpr;
};

/* Base class for an AddExpr with two operands */
class AddExprTwoOps : public AddExpr {
public:
    AddExprTwoOps(MulExpr & leftExpr,
                  AddExpr & rightExpr
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
    
    MulExpr & mLeftExpr;
    AddExpr & mRightExpr;
    
#warning FIXME - Codegen
#if 0
private:
    const DTCodegenBinaryOpFunc         mCodegenBinaryOpFunc;
    const DTCodegenConstBinaryOpFunc    mCodegenConstBinaryOpFunc;
#endif
};

/* MulExpr + AddExpr */
class AddExprAdd final : public AddExprTwoOps {
public:
    AddExprAdd(MulExpr & leftExpr, AddExpr & rightExpr);
};

/* MulExpr - AddExpr */
class AddExprSub final : public AddExprTwoOps {
public:
    AddExprSub(MulExpr & leftExpr, AddExpr & rightExpr);
};

/* MulExpr | AddExpr */
class AddExprBOr final : public AddExprTwoOps {
public:
    AddExprBOr(MulExpr & leftExpr, AddExpr & rightExpr);
};

/* MulExpr ^ AddExpr */
class AddExprBXor final : public AddExprTwoOps {
public:
    AddExprBXor(MulExpr & leftExpr, AddExpr & rightExpr);
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
