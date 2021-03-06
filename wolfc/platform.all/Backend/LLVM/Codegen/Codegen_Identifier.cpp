//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../Value.hpp"
#include "AST/Nodes/Identifier.hpp"
#include "Assert.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::Identifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Search for a value declared in the current scope or parent scopes.
    // When found just return the loaded value:
    const Value * value = mCtx.lookupValueByName(astNode.name());
    
    if (!value) {
        // Couldn't find any value with this name, issue a compile error:
        mCtx.error(astNode,
                   "No symbol named '%s' (variable, function etc.) in the current scope! "
                   "Can't lookup the address of this symbol!",
                   astNode.name());
        
        // Can't do anything else, bail out:
        return;
    }
    
    if (!value->mLLVMVal) {
        // There must be an LLVM value, issue an error:
        mCtx.error(astNode,
                   "Error: can't load the value of symbol '%s' as it has no LLVM value!",
                   astNode.name());
        
        // Can't do anything else, bail out:
        return;
    }
    
    // If the value requires a load then do that first, otherwise just push the value
    if (value->mRequiresLoad) {
        WC_ASSERT(value->mLLVMVal);
        llvm::Value * loadedValue = mCtx.mIRBuilder.CreateLoad(value->mLLVMVal, "Identifier:Load");
        WC_ASSERT(loadedValue);
        mCtx.pushValue(Value(loadedValue, value->mCompiledType, false, &astNode));
    }
    else {
        mCtx.pushValue(*value);
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
