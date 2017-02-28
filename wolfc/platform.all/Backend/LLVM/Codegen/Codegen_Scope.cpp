//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/Scope.hpp"
#include "AST/Nodes/Stmnt.hpp"
#include "Assert.hpp"
#include "Finally.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::Scope & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Record the scope being visited in addition to the node
    mCtx.pushScope(astNode);
    Finally popScope([&]{ mCtx.popScope(); });
    
    // Codegen all statements within the scope
    for (const AST::Stmnt * stmnt : astNode.getStmnts()) {
        WC_ASSERT(stmnt);
        stmnt->accept(*this);
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
