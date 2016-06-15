#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Constant;
    class Type;
    class Value;
}

WC_BEGIN_NAMESPACE

class CodegenCtx;
class PrintStmnt;

/* Base class for a data type */
class DataType {
public:
    virtual ~DataType();
    
    /* Give the name of the datatype, e.g 'int32' */
    virtual const char * name() const = 0;
    
    /* Tells if this data type matches another. */
    virtual bool equals(const DataType & other) const = 0;
    
    /**
     * Return the llvm type for this data type, or nullptr if there is no direct llvm type.
     * Note: the code generation context is required to get this info.
     */
    virtual llvm::Type * llvmType(CodegenCtx & cgCtx) const = 0;
    
    /**
     * Generate the code for a print statement for this type. The codegen context, printf function prototype, 
     * parent print statement and the value to be printed are passed in as params.
     */
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const = 0;
};

WC_END_NAMESPACE
