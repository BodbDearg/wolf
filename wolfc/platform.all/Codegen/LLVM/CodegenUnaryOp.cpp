#include "CodegenUnaryOp.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCIExpr.hpp"
#include "Codegen.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

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
    
    if (nodeAsExpr) {
        mExprType = &nodeAsExpr->dataType();
    }
    else {
        mCG.mCtx.error(mExpr,
                       "Operand of '%s' (%s) operator must be an expression with a value result!",
                       mOpSymbol,
                       mOpName);
    }
    
    // Okay, codegen the operand expression
    mExpr.accept(mCG);
    mExprVal = mCG.mCtx.popLLVMValue();
    
    // Must have a generated type and value to proceed any further:
    WC_GUARD(mExprType && mExprVal);
    mExprType->accept(*this);
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
    WC_GUARD(mExprType);
    AST::ASTNode * parent = mExpr.mParent;
    WC_ASSERT(parent);
    mCG.mCtx.error(*parent,
                   "Unary operator '%s' (%s) is not supported for an expression of type '%s'!",
                   mOpSymbol,
                   mOpName,
                   mExprType->name().c_str());
}

void CodegenUnaryOp::pushOpResult(llvm::Value * result) {
    WC_GUARD_ASSERT(result);
    mCG.mCtx.pushLLVMValue(*result);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
