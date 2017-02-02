#include "CodegenUnaryOp.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/ASTNode.hpp"
#include "AST/Nodes/IExpr.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenUnaryOp::CodegenUnaryOp(Codegen & cg,
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

void CodegenUnaryOp::codegen() {
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

void CodegenUnaryOp::visit(const ArrayBadSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const ArrayDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const ArrayUnevalSizeDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const FuncDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const FuncUnevalDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const UnknownDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    issueUnaryOpNotSupportedError();
}

void CodegenUnaryOp::issueUnaryOpNotSupportedError() {
    AST::ASTNode * parent = mExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Unary operator '%s' (%s) is not supported for an expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mExprVal.mCompiledType.getDataType().name().c_str());
}

void CodegenUnaryOp::pushOpResult(llvm::Value * result) {
    pushOpResult(result, false);
}

void CodegenUnaryOp::pushOpResult(llvm::Value * result, bool requiresLoad) {
    pushOpResult(result, requiresLoad, mExprVal.mCompiledType);
}

void CodegenUnaryOp::pushOpResult(llvm::Value * result, bool requiresLoad, const CompiledDataType & resultType) {
    mCG.mCtx.pushValue(Value(result, resultType, requiresLoad, mExpr.mParent));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
