#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class BinaryExpr;
class DataType;
class StrLit;

/*
ReadnumExpr
    readnum ( )
*/
class ReadnumExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static ReadnumExpr * parse(const Token *& tokenPtr);
    
    ReadnumExpr(const Token & startToken, const Token & endToken);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    /* Return the data type of this literal */
    const DataType & getDataType() const;
    
    const Token & mStartToken;
    const Token & mEndToken;
};

WC_END_NAMESPACE
