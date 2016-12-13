#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class AssignExpr;
class DataType;
class LinearAlloc;

/*
ReturnStmnt:
	return
	return AssignExpr
    return if|unless AssignExpr
    return AssignExpr if|unless AssignExpr
*/
class ReturnStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static ReturnStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    ReturnStmnt(const Token & returnToken);
    
    virtual const Token & getStartToken() const final override;
    
    /* Gives the data type of the value returned */
    virtual DataType & dataType() = 0;
    
    /* The token for 'return' */
    const Token & mReturnToken;
    
protected:
    /* Perform code generation and verification for the return data type */
    bool codegenAndVerifyReturnDataType(CodegenCtx & cgCtx);
    
    /**
     * Verifies the return type is correct for the containing function.
     * If incorrect returns false and emits a compile error.
     */
    bool verifyReturnTypeCorrect();
};

/* return */
class ReturnStmntNoCondVoid final : public ReturnStmnt {
public:
    ReturnStmntNoCondVoid(const Token & returnToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual DataType & dataType() override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
};

/* return AssignExpr */
class ReturnStmntNoCondWithValue final : public ReturnStmnt {
public:
    ReturnStmntNoCondWithValue(const Token & returnToken, AssignExpr & returnExpr);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual DataType & dataType() override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /* Expression for the value to return */
    AssignExpr & mReturnExpr;
};

/* This is a base class for return statements with a condition */
class ReturnStmntWithCondBase : public ReturnStmnt {
public:
    ReturnStmntWithCondBase(const Token & returnToken,
                            const Token & condToken,
                            AssignExpr & condExpr);
    
    virtual const Token & getEndToken() const final override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
    /* Returns true if the condition for returning is inversed ('unless' instead of 'if') */
    bool isCondExprInversed() const;

    /* Is either 'if' or 'unless' */
    const Token & mCondToken;
    
    /* Expression for the return condition */
    AssignExpr & mCondExpr;
    
protected:
    /* The block that does the return statement logic */
    llvm::BasicBlock * mReturnBlock = nullptr;
    
    /* The block that continues and doesn't return */
    llvm::BasicBlock * mContinueBlock = nullptr;
};

/* return if|unless AssignExpr */
class ReturnStmntWithCondVoid final : public ReturnStmntWithCondBase {
public:
    ReturnStmntWithCondVoid(const Token & returnToken,
                            const Token & condToken,
                            AssignExpr & condExpr);
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual DataType & dataType() override;
};

/* return AssignExpr if|unless AssignExpr */
class ReturnStmntWithCondAndValue final : public ReturnStmntWithCondBase {
public:
    ReturnStmntWithCondAndValue(const Token & returnToken,
                                AssignExpr & returnExpr,
                                const Token & condToken,
                                AssignExpr & condExpr);
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    virtual DataType & dataType() override;

    /* Expression for the value to return */
    AssignExpr & mReturnExpr;
};

WC_END_NAMESPACE
