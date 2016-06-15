#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class DataValue;

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
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    const char * name() const;
    
    const DataValue * lookupDataValue() const;
    
    const Token & mToken;
};

WC_END_NAMESPACE
