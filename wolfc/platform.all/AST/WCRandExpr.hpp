#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class AssignExpr;
class DataType;
class LinearAlloc;
class StrLit;

/*
RandExpr:
    rand ( )
	srand ( AssignExpr )
*/
class RandExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static RandExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    RandExpr(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const final override;
    virtual const Token & getEndToken() const final override;
    
    virtual bool isLValue() final override;
    virtual bool isConstExpr() final override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) final override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) final override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

/* rand ( ) */
class RandExprRand final : public RandExpr {
public:
    RandExprRand(const Token & startToken, const Token & endToken);
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
};

/* srand ( AssignExpr ) */
class RandExprSRand final : public RandExpr {
public:
    RandExprSRand(const Token & startToken,
                  AssignExpr & seedExpr,
                  const Token & endToken);
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    AssignExpr & mSeedExpr;
};

WC_END_NAMESPACE