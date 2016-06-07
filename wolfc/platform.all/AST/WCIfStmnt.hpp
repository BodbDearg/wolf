#pragma once

#include "WCASTNode.hpp"
#include "WCIStmnt.hpp"

namespace llvm {
    class BasicBlock;
}

WC_BEGIN_NAMESPACE

class DataType;
class AssignExpr;

/*
IfStmnt:
	if AssignExpr then Scope end
	if AssignExpr then Scope else IfStmnt
	if AssignExpr then Scope else Scope end
*/
class IfStmnt : public ASTNode, public IStmnt {
public:
    static bool peek(const Token * tokenPtr);
    
    static IfStmnt * parse(const Token *& tokenPtr);
    
    IfStmnt(AssignExpr & ifExpr,
            Scope & thenScope,
            const Token & startToken);
    
    virtual const Token & getStartToken() const override;
    
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
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
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
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
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
    
    virtual bool codegenStmnt(const CodegenCtx & cgCtx) override;
    
    Scope &         mElseScope;
    const Token &   mEndToken;
};

WC_END_NAMESPACE
