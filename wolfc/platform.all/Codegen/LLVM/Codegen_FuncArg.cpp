#include "Codegen.hpp"

#include "AST/Nodes/WCFuncArg.hpp"
#include "AST/Nodes/WCType.hpp"
#include "CodegenCtx.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::FuncArg & astNode) {
    // Codegen the type for the func arg and save it on the stack for other codegen functions
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mType.accept(*this);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
