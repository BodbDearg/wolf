#pragma once

#include "WCASTNode.hpp"
#include "WCIExpr.hpp"
#include "WCArrayDataType.hpp"

WC_BEGIN_NAMESPACE

class ArrayLitExprs;
class LinearAlloc;

/*
ArrayLit:
    [ ArrayLitExprs ]
*/
class ArrayLit : public ASTNode, public IExpr {
public:
    static bool peek(const Token * tokenPtr);
    
    static ArrayLit * parse(const Token *& tokenPtr, LinearAlloc & alloc);
    
    ArrayLit(const Token & lBrack,
             ArrayLitExprs & exprs,
             const Token & rBrack);
    
    virtual const Token & getStartToken() const override;
    
    virtual const Token & getEndToken() const override;
    
    virtual bool isLValue() const override;
    
    virtual DataType & dataType() override;
    
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) override;
    
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) override;
    
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) override;
    
    /**
     * Does the codegen for the element type. 
     * Issues a compile error and returns false if failed.
     */
    bool codegenElementType(CodegenCtx & cgCtx);
    
    const Token &       mLBrack;
    ArrayLitExprs &     mExprs;
    const Token &       mRBrack;
    size_t              mSize;
    ArrayDataType       mElementType;
};

WC_END_NAMESPACE
