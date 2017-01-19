#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE

#warning FIXME - Codegen
#if 0
struct DataValue;
#endif

WC_AST_BEGIN_NAMESPACE

/* Represents TokenType::kIdentifier in the AST tree */
class Identifier final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static Identifier * parse(ParseCtx & parseCtx);
    
    Identifier(const Token & token);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() override;
    virtual bool isConstExpr() override;
    
    virtual DataType & dataType() override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
    const char * name() const;
    
#warning FIXME - Codegen
#if 0
    DataValue * lookupDataValue();
#endif
    
    const Token & mToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
