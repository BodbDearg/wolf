#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
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

/* TernaryExpr += AssignExpr */
class AssignExprAssignAdd final : public AssignExprAssignBase {
public:
    AssignExprAssignAdd(TernaryExpr & leftExpr, AssignExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr -= AssignExpr */
class AssignExprAssignSub final : public AssignExprAssignBase {
public:
    AssignExprAssignSub(TernaryExpr & leftExpr, AssignExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr |= AssignExpr */
class AssignExprAssignBOr final : public AssignExprAssignBase {
public:
    AssignExprAssignBOr(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr ^= AssignExpr */
class AssignExprAssignBXor final : public AssignExprAssignBase {
public:
    AssignExprAssignBXor(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr *= AssignExpr */
class AssignExprAssignMul final : public AssignExprAssignBase {
public:
    AssignExprAssignMul(TernaryExpr & leftExpr, AssignExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr /= AssignExpr */
class AssignExprAssignDiv final : public AssignExprAssignBase {
public:
    AssignExprAssignDiv(TernaryExpr & leftExpr, AssignExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr %= AssignExpr */
class AssignExprAssignMod final : public AssignExprAssignBase {
public:
    AssignExprAssignMod(TernaryExpr & leftExpr, AssignExpr & rightExpr);

    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr &= AssignExpr */
class AssignExprAssignBAnd final : public AssignExprAssignBase {
public:
    AssignExprAssignBAnd(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr <<= AssignExpr */
class AssignExprAssignLShift final : public AssignExprAssignBase {
public:
    AssignExprAssignLShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr >>= AssignExpr */
class AssignExprAssignArithRShift final : public AssignExprAssignBase {
public:
    AssignExprAssignArithRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* TernaryExpr >>>= AssignExpr */
class AssignExprAssignLogicRShift final : public AssignExprAssignBase {
public:
    AssignExprAssignLogicRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

WC_END_NAMESPACE
