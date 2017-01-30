#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "AST/Nodes/Type.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::TypeArray & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.dataType().accept(mCodegenDataType);
}

void ConstCodegen::visit(const AST::TypePrimitive & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mType.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
