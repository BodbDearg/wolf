#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/WCPrimitiveType.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrimitiveType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.dataType().accept(mCodegenDataType);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
