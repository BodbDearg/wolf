#include "AddrCodegen.hpp"

#include "AST/Nodes/WCIdentifier.hpp"
#include "Codegen.hpp"
#include "CodegenCtx.hpp"
#include "ValHolder.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

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

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
