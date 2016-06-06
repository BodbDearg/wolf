#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class StrLit;

/*
ReadnumExpr
    readnum ( )
*/
class ReadnumExpr : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static ReadnumExpr * parse(const Token *& tokenPtr);
    
    ReadnumExpr(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

WC_END_NAMESPACE
