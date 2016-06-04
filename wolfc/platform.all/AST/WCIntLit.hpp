#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class DataType;

/* Represents TokenType::kIntLit in the AST tree */
class IntLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static IntLit * parse(const Token *& tokenPtr);
    
    IntLit(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    /* Return the data type of this literal */
    const DataType & getDataType() const;
    
    const Token & mToken;
};

WC_END_NAMESPACE
