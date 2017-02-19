#include "CodegenConstUnaryOp.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "Assert.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenConstUnaryOp::CodegenConstUnaryOp(ConstCodegen & cg,
                                         const AST::ASTNode & expr,
                                         const char * opSymbol,
                                         const char * opName)
:
    mCG(cg),
    mExpr(expr),
    mOpSymbol(opSymbol),
    mOpName(opName)
{
    // Expected that the node has a parent!
    WC_ASSERT(mExpr.mParent);
}

void CodegenConstUnaryOp::codegen() {
    // Get the type for the operand:
    const AST::IExpr * nodeAsExpr = dynamic_cast<const AST::IExpr*>(&mExpr);
    
    if (!nodeAsExpr) {
        mCG.mCtx.error(mExpr,
                       "Operand of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Okay, codegen the operand expression
    mExpr.accept(mCG);
    mExprConst = mCG.mCtx.popConstant();
    
    // Must have a generated type and value to proceed any further:
    if (mExprConst.isValid()) {
        const DataType & exprType = mExprConst.mCompiledType.getDataType();
        exprType.accept(*this);
    }
}

#define WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(DataTypeName)\
    void CodegenConstUnaryOp::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        issueUnaryOpNotSupportedError();\
    }

WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Array)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Bool)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Func)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int128)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int16)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int32)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int64)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Int8)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Invalid)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Str)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt128)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt16)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt32)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt64)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(UInt8)
WC_IMPL_CONST_UNARY_OP_NOT_SUPPORTED_FOR_TYPE(Void)

void CodegenConstUnaryOp::issueUnaryOpNotSupportedError() {
    AST::ASTNode * parent = mExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Unary operator '%s' (%s) is not supported for an expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mExprConst.mCompiledType.getDataType().name().c_str());
}

void CodegenConstUnaryOp::pushOpResult(llvm::Constant * result) {
    pushOpResult(result, mExprConst.mCompiledType);
}

void CodegenConstUnaryOp::pushOpResult(llvm::Constant * result, const CompiledDataType & resultType) {
    WC_ASSERT(result);
    mCG.mCtx.pushConstant(Constant(result, resultType, mExpr.mParent));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
