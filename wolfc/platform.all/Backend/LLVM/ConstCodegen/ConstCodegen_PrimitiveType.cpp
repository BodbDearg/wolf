#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::PrimitiveType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCodegenDataType.visitASTNode(astNode);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
