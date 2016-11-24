#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class TernaryExpr;

/*
ArrayLitExprs:
    TernaryExpr
    TernaryExpr , ArrayLitExprs
*/
class ArrayLitExprs : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static ArrayLitExprs * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    virtual bool isConstExpr() const = 0;
    
    virtual size_t numExprs() const = 0;
    virtual void getExprs(std::vector<TernaryExpr*> & exprs) const = 0;
    
    /**
     * Return the element type for each element. Will return the 'unknown' type if we can't
     * determine due to ambiguity.
     */
    virtual DataType & getElementType() const = 0;
};

/* TernaryExpr */
class ArrayLitExprsSingle final : public ArrayLitExprs {
public:
    ArrayLitExprsSingle(TernaryExpr & expr);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isConstExpr() const override;
    
    virtual size_t numExprs() const override;
    virtual void getExprs(std::vector<TernaryExpr*> & exprs) const override;
    
    virtual DataType & getElementType() const override;
    
    TernaryExpr & mExpr;
};

/* TernaryExpr , ArrayLitExprs */
class ArrayLitExprsMulti final : public ArrayLitExprs {
public:
    ArrayLitExprsMulti(TernaryExpr & expr, ArrayLitExprs & exprsList);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isConstExpr() const override;
    
    virtual size_t numExprs() const override;
    virtual void getExprs(std::vector<TernaryExpr*> & exprs) const override;
    
    virtual DataType & getElementType() const override;
    
    TernaryExpr &       mExpr;
    ArrayLitExprs &     mExprsList;
};

WC_END_NAMESPACE
