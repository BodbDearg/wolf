#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class DataValue;
class LinearAlloc;

/* Represents TokenType::kIdentifier in the AST tree */
class Identifier final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static Identifier * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    Identifier(const Token & token);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    const char * name() const;
    
    DataValue * lookupDataValue();
    
    const Token & mToken;
};

WC_END_NAMESPACE
