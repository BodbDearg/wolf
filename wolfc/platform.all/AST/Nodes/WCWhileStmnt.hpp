#pragma once

#include "WCASTNode.hpp"
#include "WCIRepeatableStmnt.hpp"

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class AssignExpr;

/*
WhileStmnt:
	while|until AssignExpr [do] Scope end
*/
class WhileStmnt final : public ASTNode, public IRepeatableStmnt {
public:
    static bool peek(const Token * tokenPtr);
    static WhileStmnt * parse(ParseCtx & parseCtx);
    
    WhileStmnt(AssignExpr & whileExpr,
               Scope & bodyScope,
               const Token & startToken,
               const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegen(CodegenCtx & cgCtx) override;
#endif
    
    virtual bool allCodepathsHaveUncondRet() const override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::BasicBlock * getNextStmntTargetBlock() override;
    virtual llvm::BasicBlock * getBreakStmntTargetBlock() override;
#endif
    
    /**
     * If true this is an 'until' statement rather than 'while'. In that case
     * The block will execute while the loop condition is false rather than true.
     */
    bool isWhileExprInversed() const;
    
#warning FIXME - Codegen
#if 0
    /**
     * Generate the code for the while statment condition expression and return the value of that expression.
     * Returns nullptr on failure.
     */
    llvm::Value * codegenWhileExpr(CodegenCtx & cgCtx) const;
#endif
    
    AssignExpr &            mWhileExpr;
    Scope &                 mBodyScope;
    const Token &           mStartToken;
    const Token &           mEndToken;

#warning FIXME - Codegen
#if 0
    llvm::BasicBlock *      mWhileCondBB = nullptr;
    llvm::BasicBlock *      mEndBB = nullptr;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
