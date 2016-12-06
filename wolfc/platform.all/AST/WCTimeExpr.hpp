#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;
class StrLit;

/*
TimeExpr:
    time ( )
*/
class TimeExpr final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static TimeExpr * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    TimeExpr(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

WC_END_NAMESPACE