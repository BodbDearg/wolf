//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenUnaryOp_PrefixExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CodegenLNotUnaryOp
//-----------------------------------------------------------------------------
CodegenPlusUnaryOp::CodegenPlusUnaryOp(Codegen & cg,
                                       const AST::ASTNode & expr,
                                       bool storeResultInExpr)
:
    CodegenBasicUnaryOp(cg,
                        expr,
                        "+",
                        "plus",
                        storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

#define WC_IMPL_PLUS_UNARY_OP(DataTypeName)\
    void CodegenPlusUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        pushOpResult(mExprVal.mLLVMVal);\
    }

WC_IMPL_PLUS_UNARY_OP(Int128)
WC_IMPL_PLUS_UNARY_OP(Int16)
WC_IMPL_PLUS_UNARY_OP(Int32)
WC_IMPL_PLUS_UNARY_OP(Int64)
WC_IMPL_PLUS_UNARY_OP(Int8)
WC_IMPL_PLUS_UNARY_OP(UInt128)
WC_IMPL_PLUS_UNARY_OP(UInt16)
WC_IMPL_PLUS_UNARY_OP(UInt32)
WC_IMPL_PLUS_UNARY_OP(UInt64)
WC_IMPL_PLUS_UNARY_OP(UInt8)

#undef WC_IMPL_PLUS_UNARY_OP

//-----------------------------------------------------------------------------
// CodegenLNotUnaryOp
//-----------------------------------------------------------------------------
CodegenMinusUnaryOp::CodegenMinusUnaryOp(Codegen & cg,
                                         const AST::ASTNode & expr,
                                         bool storeResultInExpr)
:
    CodegenBasicUnaryOp(cg,
                        expr,
                        "-",
                        "minus",
                        storeResultInExpr)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int128, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int16, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int32, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int64, CreateNeg)
WC_IMPL_BASIC_UNARY_OP(CodegenMinusUnaryOp, Int8, CreateNeg)

#define WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(DataTypeName)\
    void CodegenMinusUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        mCG.mCtx.error("Negation operator (-) is not allowed for an expression of unsigned "\
                       "data type '%s'!",\
                       mExprVal.mCompiledType.getDataType().name().c_str());\
    }

WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt128)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt16)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt32)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt64)
WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE(UInt8)

#undef WC_IMPL_NEG_NOT_ALLOWED_FOR_UNSIGNED_TYPE

//-----------------------------------------------------------------------------
// CodegenAddrOfUnaryOp
//-----------------------------------------------------------------------------
CodegenAddrOfUnaryOp::CodegenAddrOfUnaryOp(Codegen & cg, const AST::ASTNode & expr)
:
    CodegenBasicUnaryOp(cg,
                        expr,
                        "&",
                        "address of",
                        false)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenAddrOfUnaryOp::codegen() {
    // Sanity check, this is not allowed for this op type:
    if (mStoreResultInExpr) {
        mCG.mCtx.error("Internal error! Storing the result of a unary 'address of' op in "
                       "the same operand is not allowed!");
        
        return;
    }
    
    // Get the type for the operand:
    const AST::IExpr * nodeAsExpr = dynamic_cast<const AST::IExpr*>(&mExpr);
    
    if (!nodeAsExpr) {
        mCG.mCtx.error(mExpr,
                       "Operand of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Okay, codegen the address of the operand expression. If that fails then bail out.
    mExpr.accept(mCG.mAddrCodegen);
    Value exprAddrVal = mCG.mCtx.popValue();
    WC_GUARD(exprAddrVal.isValid());
    
    // Create the compiled data type for the result expression
    //
    // TODO: What allocates this memory?
    PtrDataType * ptrDataType = new PtrDataType(exprAddrVal.mCompiledType.getDataType(), false);
    ptrDataType->accept(mCG.mCodegenDataType);
    CompiledDataType addressOfExprCDT = mCG.mCtx.popCompiledDataType();
    
    // Okay, push the result if all is well
    mCG.mCtx.pushValue(Value(exprAddrVal.mLLVMVal, addressOfExprCDT, false, mCG.mCtx.getCurrentASTNode()));
}

//-----------------------------------------------------------------------------
// CodegenPtrDerefUnaryOp
//-----------------------------------------------------------------------------
CodegenPtrDerefUnaryOp::CodegenPtrDerefUnaryOp(Codegen & cg,
                                               const AST::ASTNode & expr,
                                               bool loadExprResult)
:
    CodegenBasicUnaryOp(cg,
                        expr,
                        "$",
                        "dereference pointer",
                        false),
    mLoadExprResult(loadExprResult)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenPtrDerefUnaryOp::codegen() {
    // Sanity check, this is not allowed for this op type:
    if (mStoreResultInExpr) {
        mCG.mCtx.error("Internal error! Storing the result of a unary 'pointer dereference' op in "
                       "the same operand is not allowed!");
        
        return;
    }
    
    // Get the type for the operand:
    const AST::IExpr * nodeAsExpr = dynamic_cast<const AST::IExpr*>(&mExpr);
    
    if (!nodeAsExpr) {
        mCG.mCtx.error(mExpr,
                       "Operand of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Evaluate the operand expression. If that fails then bail out:
    mExpr.accept(mCG);
    Value exprVal = mCG.mCtx.popValue();
    WC_GUARD(exprVal.isValid());
    
    // The expression must be a pointer type:
    const CompiledDataType & exprCDT = exprVal.mCompiledType;
    const DataType & exprDataType = exprCDT.getDataType();
    
    if (!exprDataType.isPtr()) {
        mCG.mCtx.error("Can only dereference an expression that is of a pointer type! The expression type ('%s') "
                       "being dereferenced is not a pointer!",
                       exprDataType.name().c_str());
        
        return;
    }
    
    const PtrDataType & ptrDataType = static_cast<const PtrDataType&>(exprDataType);
    
    // Makeup the data type for the returned value
    CompiledDataType dereferencedValCDT(ptrDataType.mPointedToType,
                                        exprCDT.getLLVMType()->getPointerElementType());
    
    // Makeup the return value:
    if (mLoadExprResult) {
        // Require a load of the result - do it and return that as the result
        llvm::Value * derefLLVMVal = mCG.mCtx.mIRBuilder.CreateLoad(exprVal.mLLVMVal, "CodegenPtrDerefUnaryOp:Load");
        mCG.mCtx.pushValue(Value(derefLLVMVal, dereferencedValCDT, false, mCG.mCtx.getCurrentASTNode()));
    }
    else {
        // No load required, just use the expression result directly but mark the value as needing a load.
        // This codepath will be used on the left side of an assign expression.
        mCG.mCtx.pushValue(Value(exprVal.mLLVMVal, dereferencedValCDT, true, mCG.mCtx.getCurrentASTNode()));
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
