#pragma once

#warning FIXME - Codegen
#if 0
#include "DataType/WCDataTypeCodegenFuncs.hpp"
#endif

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

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
    static AssignExpr * parse(ParseCtx & parseCtx);
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
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
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

#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) final override;
#endif
    
    TernaryExpr &   mLeftExpr;
    AssignExpr &    mRightExpr;

#warning FIXME - Codegen
#if 0
protected:
    /**
     * Does basic checks at compile time to make sure the assign can be performed.
     * Returns false and issues a compile error if the assign is illegal.
     */
     bool compileCheckAssignIsLegal();
#endif
};

/* TernaryExpr = AssignExpr */
class AssignExprAssign final : public AssignExprAssignBase {
public:
    AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
#endif
};

/* Base class for an assign expression that does a binary operation (add, mul etc.) */
class AssignExprBinaryOpBase : public AssignExprAssignBase {
public:
    AssignExprBinaryOpBase(TernaryExpr & leftExpr,
                           AssignExpr & rightExpr
                        #warning FIXME - Codegen
                        #if 0
                           ,DTCodegenBinaryOpFunc codegenBinaryOpFunc
                        #endif
                           );
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) final override;
    
    const DTCodegenBinaryOpFunc mCodegenBinaryOpFunc;
#endif
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

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
