//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "../Constant.hpp"
#include "AST/Nodes/StrLit.hpp"
#include "Assert.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::StrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the string
    {
        const DataType & dataType = PrimitiveDataTypes::getUsingTypeId(DataTypeId::kStr);
        dataType.accept(mCodegenDataType);
    }
    
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Create a constant for the string array:
    llvm::Constant * strArray = llvm::ConstantDataArray::getString(
        mCtx.mLLVMCtx,
        astNode.mToken.data.strVal.ptr
    );
    
    WC_ASSERT(strArray);
    
    // TODO: memory management here - use the linear allocator
    // TODO: need some sort of LUT for the module so we only have unique string instances
    llvm::GlobalVariable * strGlobalVar = new llvm::GlobalVariable(
        *mCtx.mLLVMModule,
        strArray->getType(),
        true,                                           // Is constant
        llvm::GlobalValue::PrivateLinkage,
        strArray,                                       // Initializer
        "StrLit"
    );
    
    WC_ASSERT(strGlobalVar);
    
    // Return a constant expression to get a pointer to the first element of the string
    llvm::ConstantInt * firstIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCtx.mLLVMCtx), 0);
    std::vector<llvm::Constant*> indices = { firstIndex, firstIndex };
    
    llvm::Constant * strLitVal = llvm::ConstantExpr::getGetElementPtr(
        strArray->getType(),
        strGlobalVar,
        indices
    );
    
    // Save the value:
    mCtx.pushConstant(Constant(strLitVal, compiledType, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
