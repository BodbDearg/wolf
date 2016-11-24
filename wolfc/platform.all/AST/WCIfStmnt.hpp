#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class TernaryExpr;

/*
IfStmnt:
	if|unless TernaryExpr [then] Scope end
	if|unless TernaryExpr [then] Scope or IfStmnt
	if|unless TernaryExpr [then] Scope else Scope end
*/
class IfStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static IfStmnt * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    IfStmnt(TernaryExpr & ifExpr,
            IBasicCodegenNode & thenNode,
            const Token & startToken);
    
    virtual const Token & getStartToken() const final override;
    
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
    
    TernaryExpr &           mIfExpr;
    IBasicCodegenNode &     mThenNode;
    const Token &           mStartToken;
    llvm::BasicBlock *      mEndBasicBlock = nullptr;
};

/* if|unless TernaryExpr [then] Scope end */
class IfStmntNoElse final : public IfStmnt {
public:
    IfStmntNoElse(TernaryExpr & ifExpr,
                  IBasicCodegenNode & thenNode,
                  const Token & startToken,
                  const Token & endToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    const Token & mEndToken;
};

/* if|unless TernaryExpr [then] Scope or IfStmnt */
class IfStmntElseIf final : public IfStmnt {
public:
    IfStmntElseIf(TernaryExpr & ifExpr,
                  IBasicCodegenNode & thenNode,
                  IfStmnt & elseIfStmnt,
                  const Token & startToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    IfStmnt & mElseIfStmnt;
};

/* if|unless TernaryExpr [then] Scope else Scope end */
class IfStmntElse final : public IfStmnt {
public:
    IfStmntElse(TernaryExpr & ifExpr,
                IBasicCodegenNode & thenNode,
                IBasicCodegenNode & elseNode,
                const Token & startToken,
                const Token & endToken);
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx) override;
    
    IBasicCodegenNode &     mElseNode;
    const Token &           mEndToken;
};

WC_END_NAMESPACE
