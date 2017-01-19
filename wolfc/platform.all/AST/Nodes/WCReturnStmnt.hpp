#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

#warning FIXME - Codegen
#if 0
namespace llvm {
    class BasicBlock;
}
#endif

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class AssignExpr;

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
    static ReturnStmnt * parse(ParseCtx & parseCtx);
    
    ReturnStmnt(const Token & returnToken);
    
    virtual const Token & getStartToken() const final override;
    
    /* Gives the data type of the value returned */
    virtual DataType & dataType() = 0;
    
    /* The token for 'return' */
    const Token & mReturnToken;
    
protected:
#warning FIXME - Codegen
#if 0
    /* Perform code generation and verification for the return data type */
    bool codegenAndVerifyReturnDataType(CodegenCtx & cgCtx);
#endif
    
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
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual DataType & dataType() override;
    
    virtual bool allCodepathsHaveUncondRet() const override;
};

/* return AssignExpr */
class ReturnStmntNoCondWithValue final : public ReturnStmnt {
public:
    ReturnStmntNoCondWithValue(const Token & returnToken, AssignExpr & returnExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
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
    
#warning FIXME - Codegen
#if 0
protected:
    /* The block that does the return statement logic */
    llvm::BasicBlock * mReturnBlock = nullptr;
    
    /* The block that continues and doesn't return */
    llvm::BasicBlock * mContinueBlock = nullptr;
#endif
};

/* return if|unless AssignExpr */
class ReturnStmntWithCondVoid final : public ReturnStmntWithCondBase {
public:
    ReturnStmntWithCondVoid(const Token & returnToken,
                            const Token & condToken,
                            AssignExpr & condExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual DataType & dataType() override;
};

/* return AssignExpr if|unless AssignExpr */
class ReturnStmntWithCondAndValue final : public ReturnStmntWithCondBase {
public:
    ReturnStmntWithCondAndValue(const Token & returnToken,
                                AssignExpr & returnExpr,
                                const Token & condToken,
                                AssignExpr & condExpr);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual DataType & dataType() override;

    /* Expression for the value to return */
    AssignExpr & mReturnExpr;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
