#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

class AssignExpr;
class DataType;
class LinearAlloc;

/*
IfStmnt:
	if|unless AssignExpr [then] Scope end
	if|unless AssignExpr [then] Scope or IfStmnt
	if|unless AssignExpr [then] Scope else Scope end
*/
class IfStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static IfStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    IfStmnt(AssignExpr & ifExpr,
            Scope & thenScope,
            const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
    /**
     * If true this is an 'unless' statement rather than 'if'. In that case
     * The block will execute if the condition is false. 
     */
    bool isIfExprInversed() const;
    
    /**
     * Generate the code for the if statment condition expression and return the value of that expression.
     * Returns nullptr on failure.
     */
    llvm::Value * codegenIfExpr(CodegenCtx & cgCtx) const;
    
    AssignExpr &            mIfExpr;
    Scope &                 mThenScope;
    const Token &           mStartToken;
    llvm::BasicBlock *      mEndBasicBlock = nullptr;
};

/* if AssignExpr then Scope end */
class IfStmntNoElse : public IfStmnt {
public:
    IfStmntNoElse(AssignExpr & ifExpr,
                  Scope & thenScope,
                  const Token & startToken,
                  const Token & endToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    const Token & mEndToken;
};

/* if AssignExpr then Scope else IfStmnt */
class IfStmntElseIf : public IfStmnt {
public:
    IfStmntElseIf(AssignExpr & ifExpr,
                  Scope & thenScope,
                  IfStmnt & elseIfStmnt,
                  const Token & startToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    IfStmnt & mElseIfStmnt;
};

/* if AssignExpr then Scope else Scope end */
class IfStmntElse : public IfStmnt {
public:
    IfStmntElse(AssignExpr & ifExpr,
                Scope & thenScope,
                Scope & elseScope,
                const Token & startToken,
                const Token & endToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    Scope &         mElseScope;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
