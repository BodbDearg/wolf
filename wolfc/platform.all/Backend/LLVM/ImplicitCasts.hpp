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
class CodegenCtx;
class ConstCodegen;

/* Utilities for dealing with implicit casts performed by the compiler */
namespace ImplicitCasts {
    /* Enum representing a cast direction for a binary op */
    enum class BinaryOpCastDir {
        /* No cast possible. E.G Invalid input or can't find any possible implicit casts. */
        kNoCastPossible,
        /* No cast is required, binary op types are the same */
        kNoCastRequired,
        /* Can't determine which way to cast. */
        kCastDirIsAmbiguous,
        /* Cast the left value to the right type */
        kCastLeftToRight,
        /* Cast the right value to the left type */
        kCastRightToLeft
    };
    
    /**
     * Check if an implicit cast is allowed between the given source and destination types.
     *
     * Notes:
     * (1) If a value is given and it is a constant then that will be used to determine whether the implicit
     * cast is allowable or not; for example an int64 constant with a value of '1' will be allowed to cast
     * implicitly to an int8 because the compiler knows the data will fit without any loss.
     */
    bool canDoImplicitCast(const CompiledDataType & fromType,
                           const CompiledDataType & toType,
                           llvm::Value * fromVal = nullptr);
    
    /**
     * Functions which do an implicit cast of the given compile time constant or runtime 
     * value to the given type if possible.
     *
     * Notes:
     * (1) If the cast is not possible or allowed then the constant or value passed in
     * will be left unmodified. This allows the callee to issue errors about mismatching types etc.
     * (2) If the cast fails then an invalid constant or value is set on the value/constant passed in.
     * (3) If no cast is required then the constant or value passed in is left unmodified.
     */
    void castSingleValueIfRequired(Codegen & cg,
                                   Value & value,
                                   const CompiledDataType & toTypeCDT);
    
    void castSingleConstantIfRequired(ConstCodegen & cg,
                                      Constant & constant,
                                      const CompiledDataType & toTypeCDT);
    
    /**
     * Try to determine which way we should cast for the given binary op.
     * If the cast dir to use is ambiguous then an error will be emitted. 
     */
    BinaryOpCastDir determineCastDirForBinaryOp(CodegenCtx & cgCtx,
                                                llvm::Value & leftLLVMVal,
                                                const CompiledDataType & leftTypeCDT,
                                                llvm::Value & rightLLVMVal,
                                                const CompiledDataType & rightTypeCDT);
    
    BinaryOpCastDir determineCastDirForBinaryOp(CodegenCtx & cgCtx,
                                                const Value & leftVal,
                                                const Value & rightVal);
    
    BinaryOpCastDir determineCastDirForBinaryOp(CodegenCtx & cgCtx,
                                                const Constant & leftConst,
                                                const Constant & rightConst);
    
    /**
     * Do any required casting that is possible and allowed for the operands in runtime and
     * compile time binary expressions.
     *
     * Notes:
     * (1) If the cast is not possible or allowed then the constants or values passed in
     * will be left unmodified. This allows the callee to issue errors about mismatching types etc.
     * (2) If the cast fails then an invalid constant or value will be set for the operand
     * on which the cast failed on.
     * (3) If no cast is required then the constants or value passed in are left unmodified.
     * (4) If it's ambiguous which way to cast then an error will be emitted.
     * (5) If casts both ways are allowed for numeric types, the larger of the two types will be used
     * PROVIDED that the signs of both integer types match. If the signs do not match then it's
     * ambiguous as to which cast is preferable.
     */
    void castBinaryOpValuesIfRequired(Codegen & cg,
                                      Value & leftVal,
                                      Value & rightVal);
    
    void castBinaryOpValuesIfRequired(ConstCodegen & cg,
                                      Constant & leftConst,
                                      Constant & rightConst);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
