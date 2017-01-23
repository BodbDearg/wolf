#pragma once

#include "AST/Nodes/WCASTNode.hpp"
#include "DataType/Primitives/WCUnknownDataType.hpp"
#include "WCIExpr.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class ArrayLitExprs;

/*
ArrayLit:
    [ ArrayLitExprs ]
*/
class ArrayLit final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ArrayLit * parse(ParseCtx & parseCtx);
    
    ArrayLit(const Token & lBrack,
             ArrayLitExprs & exprs,
             const Token & rBrack);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual const DataType & dataType() const override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
#endif
    
private:
    
#warning FIXME - Codegen
#if 0
    /**
     * Does the codegen for the element llvm type. 
     * Issues a compile error and returns false if failed.
     */
    bool codegenLLVMType(CodegenCtx & cgCtx);
#endif
    
    const Token &      mLBrack;
    ArrayLitExprs &    mExprs;
    const Token &      mRBrack;
    size_t             mSize;
    UnknownDataType    mDataType;
    
#warning FIXME - Codegen
#if 0
    llvm::Value *               mStorage;
    llvm::Value *               mAddrOfResult;
    llvm::Value *               mExprEvalResult;
    llvm::Constant *            mExprConstEvalResult;
#endif
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
