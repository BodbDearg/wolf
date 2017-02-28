#include "Codegen.hpp"

#include "../CodegenBinaryOp/CodegenBinaryOp_AddExpr.hpp"
#include "../CodegenBinaryOp/CodegenBinaryOp_MulExpr.hpp"
#include "../CodegenBinaryOp/CodegenBinaryOp_ShiftExpr.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/TernaryExpr.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "DataType/Types/VoidDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::AssignExprNoAssign & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::AssignExprAssign & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the address of the left side
    astNode.mLeftExpr.accept(mAddrCodegen);
    Value leftAddr = mCtx.popValue();
    
    // Codegen the right value
    astNode.mRightExpr.accept(*this);
    Value rightVal = mCtx.popValue();
    
    // The left and right data types must match
    // TODO: Handle auto type promotion here?
    const DataType & leftDataType = leftAddr.mCompiledType.getDataType();
    const DataType & rightDataType = rightVal.mCompiledType.getDataType();
    bool typesMatch = true;
    
    if (!leftDataType.equals(rightDataType)) {
        mCtx.error(astNode,
                   "Can't assign an expression of type '%s' to a variable of type '%s'!",
                   rightDataType.name().c_str(),
                   leftDataType.name().c_str());
        
        typesMatch = false;
    }
    
    // TODO: support operator overloading here eventually
    // Do the assign if we can:
    if (typesMatch && leftAddr.mLLVMVal && rightVal.mLLVMVal) {
        // Sanity check, left type must be a pointer
        WC_ASSERT(leftAddr.mLLVMVal->getType()->isPointerTy());
        
        // Do the store:
        mCtx.mIRBuilder.CreateStore(rightVal.mLLVMVal, leftAddr.mLLVMVal);
    }
    
    // The result of an assign expression is 'void'
    PrimitiveDataTypes::getVoidDataType().accept(mCodegenDataType);
    CompiledDataType voidCDT = mCtx.popCompiledDataType();
    mCtx.pushValue(Value(nullptr, voidCDT, false, &astNode));
}

void Codegen::visit(const AST::AssignExprAssignAdd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenAddBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignSub & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenSubBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignBOr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignBXor & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBXOrBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMulBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenDivBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignRem & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenRemBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignLShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignARShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenARShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

void Codegen::visit(const AST::AssignExprAssignLRShift & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenLRShiftBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr, true).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
