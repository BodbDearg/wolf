#pragma once

#include "WCASTNode.hpp"
#include <vector>

WC_BEGIN_NAMESPACE

class AssignExpr;
class DataType;
class LinearAlloc;

/*
ArrayLitExprs:
    AssignExpr
    AssignExpr , ArrayLitExprs
*/
class ArrayLitExprs : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    
    static ArrayLitExprs * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    virtual size_t numExprs() const = 0;
    
    virtual void getExprs(std::vector<AssignExpr*> & exprs) const = 0;
    
    /**
     * Return the element type for each element. Will return the 'unknown' type if we can't
     * determine due to ambiguity.
     */
    virtual DataType & getElementType() const = 0;
};

/* AssignExpr */
class ArrayLitExprsSingle : public ArrayLitExprs {
public:
    ArrayLitExprsSingle(AssignExpr & expr);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual size_t numExprs() const override;
    
    virtual void getExprs(std::vector<AssignExpr*> & exprs) const override;
    
    virtual DataType & getElementType() const override;
    
    AssignExpr & mExpr;
};

/* AssignExpr , ArrayLitExprs */
class ArrayLitExprsMulti : public ArrayLitExprs {
public:
    ArrayLitExprsMulti(AssignExpr & expr, ArrayLitExprs & exprsList);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual size_t numExprs() const override;
    
    virtual void getExprs(std::vector<AssignExpr*> & exprs) const override;
    
    virtual DataType & getElementType() const override;
    
    AssignExpr &        mExpr;
    ArrayLitExprs &     mExprsList;
};

WC_END_NAMESPACE
