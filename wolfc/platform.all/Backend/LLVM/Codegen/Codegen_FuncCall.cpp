#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/FuncCall.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::FuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    for (const AST::AssignExpr * assignExpr : astNode.getArgs()) {
        WC_ASSERT(assignExpr);
        assignExpr->accept(*this);
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
