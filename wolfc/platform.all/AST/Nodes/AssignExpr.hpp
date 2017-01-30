#pragma once

#include "ASTNode.hpp"
#include "IExpr.hpp"

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
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
    TernaryExpr & mExpr;
};

/* Base class for assign expressions actually do an assign */
class AssignExprAssignBase : public AssignExpr {
public:
    AssignExprAssignBase(TernaryExpr & leftExpr, AssignExpr & rightExpr);

    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;

    virtual bool isLValue() const final override;
    virtual bool isConstExpr() const final override;

    virtual const DataType & dataType() const final override;
    
    TernaryExpr &   mLeftExpr;
    AssignExpr &    mRightExpr;
};

/* TernaryExpr = AssignExpr */
class AssignExprAssign final : public AssignExprAssignBase {
public:
    AssignExprAssign(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* Base class for an assign expression that does a binary operation (add, mul etc.) */
class AssignExprBinaryOpBase : public AssignExprAssignBase {
public:
    AssignExprBinaryOpBase(TernaryExpr & leftExpr, AssignExpr & rightExpr);
};

/* TernaryExpr += AssignExpr */
class AssignExprAssignAdd final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignAdd(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr -= AssignExpr */
class AssignExprAssignSub final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignSub(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr |= AssignExpr */
class AssignExprAssignBOr final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignBOr(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr ^= AssignExpr */
class AssignExprAssignBXor final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignBXor(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr *= AssignExpr */
class AssignExprAssignMul final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignMul(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr /= AssignExpr */
class AssignExprAssignDiv final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignDiv(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr %= AssignExpr */
class AssignExprAssignMod final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignMod(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr &= AssignExpr */
class AssignExprAssignBAnd final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignBAnd(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr <<= AssignExpr */
class AssignExprAssignLShift final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignLShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr >>= AssignExpr */
class AssignExprAssignARShift final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignARShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

/* TernaryExpr >>>= AssignExpr */
class AssignExprAssignLRShift final : public AssignExprBinaryOpBase {
public:
    AssignExprAssignLRShift(TernaryExpr & leftExpr, AssignExpr & rightExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
