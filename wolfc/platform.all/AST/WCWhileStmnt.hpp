#pragma once

#include "WCASTNode.hpp"
#include "WCILoopStmnt.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class AssignExpr;

/*
WhileStmnt:
	while|until AssignExpr [do] Scope end
*/
class WhileStmnt : public ASTNode, public ILoopStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static WhileStmnt * parse(const Token *& tokenPtr);
    
    WhileStmnt(AssignExpr & whileExpr,
               Scope & bodyScope,
               const Token & startToken,
               const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool codegenStmnt(CodegenCtx & cgCtx) override;
    
    virtual llvm::BasicBlock * getStartBlock() override;
    
    virtual llvm::BasicBlock * getEndBlock() override;
    
    /**
     * If true this is an 'until' statement rather than 'while'. In that case
     * The block will execute while the condition is false.
     */
    bool isWhileExprInversed() const;
    
    /**
     * Generate the code for the while statment condition expression and return the value of that expression.
     * Returns nullptr on failure.
     */
    llvm::Value * codegenWhileExpr(CodegenCtx & cgCtx) const;
    
    AssignExpr &            mWhileExpr;
    Scope &                 mBodyScope;
    const Token &           mStartToken;
    const Token &           mEndToken;
    llvm::BasicBlock *      mWhileCondBB = nullptr;
    llvm::BasicBlock *      mEndBB = nullptr;
};

WC_END_NAMESPACE
