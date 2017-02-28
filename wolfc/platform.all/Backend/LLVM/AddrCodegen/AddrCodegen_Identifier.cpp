#include "AddrCodegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "../ValHolder.hpp"
#include "AST/Nodes/Identifier.hpp"
#include "Assert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void AddrCodegen::visit(const AST::Identifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Search for a value declared in the current scope or parent scopes.
    // When found just return the value, variables are stored by address.
    const Value * value = mCtx.lookupValueByName(astNode.name());
    
    if (value) {
        mCtx.pushValue(*value);
        return;
    }
    
    // Couldn't find any value with this name, issue a compile error:
    mCtx.error(astNode,
               "No symbol named '%s' (variable, function etc.) in the current scope! "
               "Can't lookup the address of this symbol!",
               astNode.name());
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
