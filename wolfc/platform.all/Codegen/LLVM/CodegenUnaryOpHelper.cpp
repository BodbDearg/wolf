#include "CodegenUnaryOpHelper.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCIExpr.hpp"
#include "Codegen.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenUnaryOpHelper::CodegenUnaryOpHelper(Codegen & cg,
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

void CodegenUnaryOpHelper::codegen() {
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
    mExprVal = mCG.mCtx.popValue();
    
    // Must have a generated type and value to proceed any further:
    if (mExprVal.isValid()) {
        const DataType & dataType = mExprVal.mCompiledType.getDataType();
        dataType.accept(*this);
    }
}

void CodegenUnaryOpHelper::visit(const ArrayBadSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::visit(const ArrayDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::visit(const ArrayUnevalSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::visit(const UnknownDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOpHelper::issueUnaryOpNotSupportedError() {
    AST::ASTNode * parent = mExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Unary operator '%s' (%s) is not supported for an expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mExprVal.mCompiledType.getDataType().name().c_str());
}

void CodegenUnaryOpHelper::pushOpResult(llvm::Value * result) {
    pushOpResult(result, false);
}

void CodegenUnaryOpHelper::pushOpResult(llvm::Value * result, bool requiresLoad) {
    pushOpResult(result, requiresLoad, mExprVal.mCompiledType);
}

void CodegenUnaryOpHelper::pushOpResult(llvm::Value * result, bool requiresLoad, const CompiledDataType & resultType) {
    mCG.mCtx.pushValue(Value(result, resultType, requiresLoad, mExpr.mParent));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
