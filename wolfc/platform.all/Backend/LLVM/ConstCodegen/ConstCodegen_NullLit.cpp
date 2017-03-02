//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/NullLit.hpp"
#include "Assert.hpp"
#include "DataType/Types/NullptrDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::NullLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the null literal
    const NullptrDataType & nullLitDT = PrimitiveDataTypes::getNullptrDataType();
    nullLitDT.accept(mCodegenDataType);
    CompiledDataType nullLitCDT = mCtx.popCompiledDataType();
    WC_ASSERT(nullLitCDT.isValid());
    
    // Create a null pointer to i1 and save the constant to the stack
    llvm::Constant * llvmConst = llvm::Constant::getNullValue(llvm::Type::getInt1PtrTy(mCtx.mLLVMCtx));
    WC_ASSERT(llvmConst);
    mCtx.pushConstant(Constant(llvmConst, nullLitCDT, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
