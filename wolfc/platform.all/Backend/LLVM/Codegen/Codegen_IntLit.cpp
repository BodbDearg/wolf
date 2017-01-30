#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/IntLit.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::IntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the int
    {
        const DataType & dataType = PrimitiveDataTypes::getDefaultIntType();
        WC_ASSERT(dataType.isInteger());
        dataType.accept(mCodegenDataType);
    }
    
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Create the value and save to the stack
    WC_ASSERT(compiledType.isValid());
    llvm::Value * llvmValue = llvm::ConstantInt::get(compiledType.getLLVMType(), astNode.mToken.data.intVal);
    WC_ASSERT(llvmValue);
    mCtx.pushValue(Value(llvmValue, compiledType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
