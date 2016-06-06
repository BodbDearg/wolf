#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;

/* Represents TokenType::kIdentifier in the AST tree */
class Identifier : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static Identifier * parse(const Token *& tokenPtr);
    
    Identifier(const Token & token);
    
    virtual const Token & getStartToken() const override;

    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(const CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(const CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
