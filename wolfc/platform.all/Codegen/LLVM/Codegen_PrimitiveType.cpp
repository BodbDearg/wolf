#include "Codegen.hpp"

#include "AST/Nodes/WCPrimitiveType.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrimitiveType & astNode) {
    // Codegen the type for the primitive type and save it on the stack for other codegen functions
    WC_CODEGEN_RECORD_VISITED_NODE();
    DataType & dataType = astNode.dataType();
    dataType.accept(mCodegenDataType);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
