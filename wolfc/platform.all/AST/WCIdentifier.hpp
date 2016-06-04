#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class DataType;

/* Represents TokenType::kIdentifier in the AST tree */
class Identifier : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static Identifier * parse(const Token *& tokenPtr);
    
    Identifier(const Token & token);
    
    virtual const Token & getStartToken() const override;

    virtual const Token & getEndToken() const override;
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;

    llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx);
    
    /* Return the data type of this identifier */
    const DataType & getDataType() const;
    
    const Token & mToken;
};

WC_END_NAMESPACE
