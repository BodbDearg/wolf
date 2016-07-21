#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class AssignExpr;
class DataType;
class LinearAlloc;

/*
ReturnStmnt:
	return
	return AssignExpr
*/
class ReturnStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static ReturnStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    ReturnStmnt(const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    /* Gives the data type of the value returned */
    virtual const DataType & dataType() const = 0;
    
    /**
     * Verifies the return type is correct for the containing function.
     * If incorrect returns false and emits a compile error.
     */
    bool verifyReturnTypeCorrect() const;
    
    const Token & mStartToken;
};

/* return */
class ReturnStmntVoid : public ReturnStmnt {
public:
    ReturnStmntVoid(const Token & startToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual const DataType & dataType() const override;
};

/* return AssignExpr */
class ReturnStmntWithValue : public ReturnStmnt {
public:
    ReturnStmntWithValue(const Token & startToken, AssignExpr & expr);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual const DataType & dataType() const override;
    
    AssignExpr & mExpr;
};

WC_END_NAMESPACE
