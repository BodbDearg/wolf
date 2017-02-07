#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrimitiveType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.getDataType().accept(mCodegenDataType);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
