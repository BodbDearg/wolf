#include "CodegenConstUnaryOpHelper.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCIExpr.hpp"
#include "CodegenCtx.hpp"
#include "ConstCodegen.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenConstUnaryOpHelper::CodegenConstUnaryOpHelper(ConstCodegen & cg,
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

void CodegenConstUnaryOpHelper::codegen() {
    // Get the type for the operand:
    const AST::IExpr * nodeAsExpr = dynamic_cast<const AST::IExpr*>(&mExpr);
    
    if (nodeAsExpr) {
        nodeAsExpr->dataType().accept(mCG.mCodegenDataType);
        CompiledDataType exprCompiledType = mCG.mCtx.popCompiledDataType();
        mExprType = &exprCompiledType.getDataType();
    }
    else {
        mCG.mCtx.error(mExpr,
                       "Operand of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Okay, codegen the operand expression
    mExpr.accept(mCG);
    mExprVal = mCG.mCtx.popLLVMConstant();
    
    // Must have a generated type and value to proceed any further:
    WC_GUARD(mExprType && mExprVal);
    mExprType->accept(*this);
}

void CodegenConstUnaryOpHelper::visit(const ArrayBadSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::visit(const ArrayDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::visit(const ArrayUnevalSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::visit(const UnknownDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenConstUnaryOpHelper::issueUnaryOpNotSupportedError() {
    WC_GUARD(mExprType);
    AST::ASTNode * parent = mExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Unary operator '%s' (%s) is not supported for an expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mExprType->name().c_str());
}

void CodegenConstUnaryOpHelper::pushOpResult(llvm::Constant * result) {
    WC_GUARD_ASSERT(result);
    mCG.mCtx.pushLLVMConstant(*result);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
