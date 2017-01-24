#include "CodegenDataType.hpp"

#include "CodegenCtx.hpp"
#include "CompiledDataType.hpp"
#include "DataType/Primitives/WCBoolDataType.hpp"
#include "DataType/Primitives/WCInt64DataType.hpp"
#include "DataType/Primitives/WCStrDataType.hpp"
#include "DataType/Primitives/WCVoidDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenDataType::CodegenDataType(CodegenCtx & ctx) : mCtx(ctx) {
    WC_EMPTY_FUNC_BODY();
}

void CodegenDataType::visit(const ArrayDataType & dataType) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt1Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt64Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt8PtrTy(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const UnknownArrayDataType & dataType) {
    #warning TODO: Implement this
    // We can't codegen an unknown array data type
    WC_UNUSED_PARAM(dataType);
    mCtx.error("Unable to generate the llvm type for an array with an unknown size!");
}

void CodegenDataType::visit(const UnknownDataType & dataType) {
    // We can't codegen an unknown data type
    WC_UNUSED_PARAM(dataType);
    mCtx.error("Unable to generate the llvm type for an unknown data type!");
}

void CodegenDataType::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getVoidTy(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
