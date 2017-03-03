//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "CompiledDataType.hpp"
#include "Constant.hpp"
#include "Value.hpp"

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class DataType;

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class Codegen;
class ConstCodegen;

/* Utilities for dealing with implicit casts performed by the compiler */
namespace ImplicitCasts {
    /**
     * Check if an implicit cast is allowed between the given source and destination types.
     *
     * If a value is given and it is a constant then that will be used to determine whether the implicit 
     * cast is allowable or not; for example an int64 constant with a value of '1' will be allowed to cast
     * implicitly to an int8 because the compiler knows the data will fit without any loss.
     */
    bool canDoImplicitCast(const CompiledDataType & fromType,
                           const CompiledDataType & toType,
                           llvm::Value * fromVal = nullptr);
    
    /**
     * Does an implicit cast of the given constant to the given type if possible.
     *
     * If the cast is not possible no cast is performed and the constant passed in is returned.
     * If the cast fails then an invalid constant is returned.
     * If no cast is required then the constant passed in is returned.
     */
    Value castSingleValueIfRequired(Codegen & cg,
                                    const Value & value,
                                    const CompiledDataType & toTypeCDT);
    
    Constant castSingleConstantIfRequired(ConstCodegen & cg,
                                          const Constant & constant,
                                          const CompiledDataType & toTypeCDT);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
