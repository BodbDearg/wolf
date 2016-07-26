#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

class DataType;
class LinearAlloc;

/* Represents TokenType::kIntLit in the AST tree */
class IntLit : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static IntLit * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    IntLit(const Token & token);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual const DataType & dataType() const override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    const Token & mToken;
};

WC_END_NAMESPACE
