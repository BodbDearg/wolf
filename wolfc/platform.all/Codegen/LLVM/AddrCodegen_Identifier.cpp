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
    
    // Search through the scope stack for a value declared in the scope.
    // When found just return the value, variables are stored by address.
    const std::vector<const AST::Scope*> & scopeStack = mCtx.getScopeStack();
    ssize_t scopeStackSize = static_cast<ssize_t>(scopeStack.size());
    
    for (ssize_t i = scopeStackSize - 1; i >= 0; --i) {
        const AST::Scope * scope = scopeStack[static_cast<size_t>(i)];
        WC_ASSERT(scope);
        ValHolder & varHolder = mCtx.getScopeValHolder(*scope);
        const Value * value = varHolder.getVal(astNode.name());
        
        if (value) {
            // Found a value, push it to the codegen context and finish up.
            mCtx.pushValue(*value);
            return;
        }
    }
    
    // Okay, now search in the module scope:
    const Value * value = mCtx.mModuleValHolder.getVal(astNode.name());
    
    if (value) {
        // Found a value, push it to the codegen context and finish up.
        mCtx.pushValue(*value);
        return;
    }
    
    // Couldn't find any value with this name, issue a compile error:
    mCtx.error(astNode,
               "No symbol named '%s' (variable, function etc.) in the current scope!",
               astNode.name());
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
