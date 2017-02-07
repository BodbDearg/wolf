#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "AST/Nodes/Type.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::TypeArray & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.dataType().accept(mCodegenDataType);
}

void Codegen::visit(const AST::TypePrimitive & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.dataType().accept(mCodegenDataType);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
