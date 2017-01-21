#include "Codegen.hpp"

#include "AST/Nodes/WCScope.hpp"
#include "AST/Nodes/WCStmnt.hpp"
#include "CodegenCtx.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::Scope & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    for (const AST::Stmnt * stmnt : astNode.getStmnts()) {
        WC_ASSERT(stmnt);
        stmnt->accept(*this);
    }
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
