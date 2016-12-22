#pragma once

#include "DataType/WCDataTypeCodegenFuncs.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class LinearAlloc;
class TernaryExpr;

/*
AssignExpr:
    TernaryExpr
    TernaryExpr = AssignExpr
    TernaryExpr += AssignExpr
    TernaryExpr -= AssignExpr
	TernaryExpr |= AssignExpr
	TernaryExpr ^= AssignExpr
    TernaryExpr *= AssignExpr
    TernaryExpr /= AssignExpr
    TernaryExpr %= AssignExpr
	TernaryExpr &= AssignExpr
	TernaryExpr <<= AssignExpr
	TernaryExpr >>= AssignExpr
	TernaryExpr >>>= AssignExpr
*/
class AssignExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static AssignExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
};

/* TernaryExpr */
class AssignExprNoAssign final : public AssignExpr {
public:
    AssignExprNoAssign(TernaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    TernaryExpr & mExpr;
};

/* Base class for assign expressions actually do an assign */
class AssignExprAssignBase : public AssignExpr {
public:
    AssignExprAssignBase(TernaryExpr & leftExpr, AssignExpr & rightExpr);

    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;

    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;

    virtual DataType & dataType() final override;

    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) final override;

    TernaryExpr &   mLeftExpr;
    AssignExpr &    mRightExpr;

protected:
    /**
     * Does basic checks at compile time to make sure the assign can be performed.
     * Returns false and issues a compile error if the assign is illegal.
     */
     bool compileCheckAssignIsLegal();
};

/* TernaryExpr = AssignExpr */
class AssignExprAssign final : public AssignExprAssignBase {
public:
    AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* Base class for an assign expression that does a binary operation (add, mul etc.) */
class AssignExprBinaryOpBase : public AssignExprAssignBase {
public:
    AssignExprBinaryOpBase(TernaryExpr & leftExpr,
                           AssignExpr & rightExpr,
                           DTCodegenBinaryOpFunc codegenBinaryOpFunc);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) final override;
    
    const DTCodegenBinaryOpFunc mCodegenBinaryOpFunc;
};

/* TernaryExpr += AssignExpr */
class AssignExprAssignAdd final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignAdd(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr -= AssignExpr */
class AssignExprAssignSub final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignSub(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr |= AssignExpr */
class AssignExprAssignBOr final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignBOr(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr ^= AssignExpr */
class AssignExprAssignBXor final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignBXor(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr *= AssignExpr */
class AssignExprAssignMul final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignMul(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr /= AssignExpr */
class AssignExprAssignDiv final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignDiv(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr %= AssignExpr */
class AssignExprAssignMod final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignMod(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr &= AssignExpr */
class AssignExprAssignBAnd final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignBAnd(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr <<= AssignExpr */
class AssignExprAssignLShift final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignLShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr >>= AssignExpr */
class AssignExprAssignARShift final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignARShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr >>>= AssignExpr */
class AssignExprAssignLRShift final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignLRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

WC_END_NAMESPACE
