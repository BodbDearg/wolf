#pragma once

#include "WCArrayDataType.hpp"
#include "WCASTNode.hpp"
#include "WCIExpr.hpp"
#include "WCLazyInit.hpp"

WC_BEGIN_NAMESPACE

class ArrayLitExprs;
class LinearAlloc;

/*
ArrayLit:
    [ ArrayLitExprs ]
*/
class ArrayLit final : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    static ArrayLit * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    ArrayLit(const Token & lBrack,
             ArrayLitExprs & exprs,
             const Token & rBrack);
    
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    virtual bool isConstExpr() const override;
    
    virtual DataType & dataType() override;
    
    virtual bool requiresStorage() const override;
    virtual llvm::Value * getStorage() const override;
    virtual void setStorage(llvm::Value & storage) override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
private:
    /**
     * Does the codegen for the element llvm type. 
     * Issues a compile error and returns false if failed.
     */
    bool codegenLLVMType(CodegenCtx & cgCtx);
    
    const Token &               mLBrack;
    ArrayLitExprs &             mExprs;
    const Token &               mRBrack;
    size_t                      mSize;
    LazyInit<ArrayDataType>     mDataType;
    llvm::Value *               mStorage;
};

WC_END_NAMESPACE
