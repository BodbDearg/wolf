#include "Codegen.hpp"

#include "CodegenCtx.hpp"
#include "WCNoOpStmnt.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::NoOpStmnt & astNode) {
    // Like it's name suggests, 'noop' does nothing...
    WC_CODEGEN_RECORD_VISITED_NODE();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
