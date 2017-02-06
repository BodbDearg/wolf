#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/RandExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"

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
    #warning TODO: Codegen this node
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
