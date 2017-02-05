#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/TimeExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/DataTypeId.hpp"
#include "DataType/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::TimeExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Get 'time' C function
    llvm::Constant * timeFn = mCtx.mLLVMModule->getFunction("time");
    
    if (!timeFn) {
        mCtx.error(astNode, "Codegen failed! Can't find 'time' function!");
        return;
    }

    // TODO: Is this the same on 32-bit platforms too?
    // This is the value we will pass to the time() function, a null pointer
    llvm::Type * int64PtrTy = mCtx.mIRBuilder.getInt64Ty()->getPointerTo();
    WC_ASSERT(int64PtrTy);
    llvm::Constant * nullInt64Ptr = llvm::ConstantPointerNull::getNullValue(int64PtrTy);
    WC_ASSERT(nullInt64Ptr);
    
    // Create the call to time!
    llvm::Value * resultVal = mCtx.mIRBuilder.CreateCall(timeFn, { nullInt64Ptr }, "TimeExpr:result");
    WC_ASSERT(resultVal);
    
    // Get the datatype for the result:
    PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64).accept(mCodegenDataType);
    CompiledDataType resultType = mCtx.popCompiledDataType();
    
    // Save the result:
    mCtx.pushValue(Value(resultVal, resultType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
