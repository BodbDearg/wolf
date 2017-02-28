//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/StrLit.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::StrLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the string
    {
        const DataType & dataType = PrimitiveDataTypes::getUsingTypeId(DataTypeId::kStr);
        dataType.accept(mCodegenDataType);
    }
    
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Codegen the string literal and save
    // TODO: need some sort of LUT for the module so we only have unique string instances
    llvm::Value * strVal = mCtx.mIRBuilder.CreateGlobalStringPtr(
        astNode.mToken.data.strVal.ptr,
        "StrLit"
    );
    
    WC_ASSERT(strVal);
    mCtx.pushValue(Value(strVal, compiledType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
