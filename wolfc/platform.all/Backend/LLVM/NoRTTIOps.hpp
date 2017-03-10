//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/**
 * A set of wrapped LLVM operations which when used cause linker errors unless we disable RTTI for the host module.
 *
 * Since we want to actually use RTTI throughout the rest of this compiler and not have LLVM's no-RTTI decision 
 * forced upon us, we wrap all these troublesome operations here and disable RTTI for this specific module only.
 *
 * This allows us to use RTTI freely throughout the rest of the code as normal and isolate these 'dirty' wrappers 
 * that we use to work around the RTTI problem.
 */
namespace NoRTTIOps {
    /* Wrapped IRBuilder operations */
    namespace IRB {
        /* Wraps the llvm::IRBuilder::CreateInsertValue() */
        llvm::Value * CreateInsertValue(llvm::IRBuilder<> & irbuilder,
                                        llvm::Value & aggregate,
                                        llvm::Value & value,
                                        const llvm::ArrayRef<unsigned> & indexes,
                                        const char * label);
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
