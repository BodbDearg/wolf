#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
ArrayLitExprs:
    AssignExpr
    AssignExpr , ArrayLitExprs
*/
class ArrayLitExprs : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static ArrayLitExprs * parse(ParseCtx & parseCtx);
    
    virtual bool isConstExpr() const = 0;
    
    virtual size_t numExprs() const = 0;
    virtual void getExprs(std::vector<AssignExpr*> & exprs) const = 0;
    
    /**
     * Return the element type for each element. Will return the 'unknown' type if we can't
     * determine due to ambiguity.
     */
    virtual const DataType & getElementType() const = 0;
};

/* AssignExpr */
class ArrayLitExprsSingle final : public ArrayLitExprs {
public:
    ArrayLitExprsSingle(AssignExpr & expr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isConstExpr() const override;
    
    virtual size_t numExprs() const override;
    virtual void getExprs(std::vector<AssignExpr*> & exprs) const override;
    
    virtual const DataType & getElementType() const override;
    
    AssignExpr & mExpr;
};

/* AssignExpr , ArrayLitExprs */
class ArrayLitExprsMulti final : public ArrayLitExprs {
public:
    ArrayLitExprsMulti(AssignExpr & expr, ArrayLitExprs & exprsList);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isConstExpr() const override;
    
    virtual size_t numExprs() const override;
    virtual void getExprs(std::vector<AssignExpr*> & exprs) const override;
    
    virtual const DataType & getElementType() const override;
    
    AssignExpr &        mExpr;
    ArrayLitExprs &     mExprsList;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
