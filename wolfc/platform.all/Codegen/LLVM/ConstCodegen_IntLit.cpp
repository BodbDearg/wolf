#include "ConstCodegen.hpp"

#include "AST/Nodes/WCIntLit.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::IntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the int
    const DataType & dataType = PrimitiveDataTypes::getDefaultIntType();
    WC_ASSERT(dataType.isInteger());
    dataType.accept(mCodegenDataType);
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Create the constant and save to the stack
    WC_ASSERT(compiledType.getLLVMType());
    llvm::Constant * constant = llvm::ConstantInt::get(compiledType.getLLVMType(), astNode.mToken.data.intVal);
    WC_ASSERT(constant);
    mCtx.pushConstant(Constant(constant, compiledType, &astNode));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE