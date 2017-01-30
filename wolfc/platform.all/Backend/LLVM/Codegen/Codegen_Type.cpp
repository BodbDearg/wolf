#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "AST/Nodes/Type.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::TypeArray & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.dataType().accept(mCodegenDataType);
}

void Codegen::visit(const AST::TypePrimitive & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mType.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
