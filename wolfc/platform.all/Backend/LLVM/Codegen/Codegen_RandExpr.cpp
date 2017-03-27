//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/RandExpr.hpp"
#include "Assert.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::RandExprRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Get 'rand' C function
    llvm::Constant * randFn = mCtx.mLLVMModule->getFunction("rand");
    
    if (!randFn) {
        mCtx.error("Codegen failed! Can't find 'rand' function!");
        return;
    }
    
    // Create the call to rand!
    llvm::Value * randResult = mCtx.mIRBuilder.CreateCall(randFn, {}, "RandExprRand:result");
    WC_ASSERT(randResult);
    
    // TODO: this might need changing for 32-bit targets
    // Codegen the int64 type:
    {
        const DataType & returnDataType = PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64);
        returnDataType.accept(mCodegenDataType);
    }
    
    CompiledDataType returnType = mCtx.popCompiledDataType();
    WC_ASSERT(returnType.isValid());
    
    // It needs to be converted to an int64:
    llvm::Value * extendedResult = mCtx.mIRBuilder.CreateSExt(randResult, returnType.getLLVMType());
    WC_ASSERT(extendedResult);
    
    // Save the result
    mCtx.pushValue(Value(extendedResult, returnType, false, &astNode));
}

void Codegen::visit(const AST::RandExprSRand & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Evaluate the seed expression:
    astNode.mSeedExpr.accept(*this);
    Value seedExprVal = mCtx.popValue();
    
    // TODO: This might need to change for 32-bit code
    // The seed expr type must be 'int'
    const DataType & seedDataType = seedExprVal.mCompiledType.getDataType();
    const DataType & requiredDataType = PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64);
    
    if (!seedDataType.equals(requiredDataType)) {
        mCtx.error("Data type for seed given to 'srand()' call must be '%s' not '%s'!",
                   requiredDataType.name().c_str(),
                   seedDataType.name().c_str());
        
        return;
    }
    
    // Don't continue if this is not valid
    WC_GUARD(seedExprVal.isValid());
    
    // Get 'srand' C function
    llvm::Constant * srandFn = mCtx.mLLVMModule->getFunction("srand");
    
    if (!srandFn) {
        mCtx.error("Codegen failed! Can't find 'srand' function!");
        return;
    }
    
    // Need to downcast the seed to a 32-bit int
    llvm::Type * int32Ty = mCtx.mIRBuilder.getInt32Ty();
    WC_ASSERT(int32Ty);
    WC_ASSERT(seedExprVal.mLLVMVal);
    llvm::Value * seedExprVal32 = mCtx.mIRBuilder.CreateTrunc(seedExprVal.mLLVMVal, int32Ty);
    WC_ASSERT(seedExprVal32);
    
    // Create the call to srand!
    llvm::Value * srandResult = mCtx.mIRBuilder.CreateCall(srandFn, { seedExprVal32 });
    WC_ASSERT(srandResult);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
