#pragma once

#include "WCMacros.hpp"
#include <string>

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
    virtual const std::string & name() const = 0;
    
    /* Tells if this data type matches another. */
    virtual bool equals(const DataType & other) const = 0;
    
    /* Tells if the data type is valid. Returns 'true' for all data types except 'unknown' */
    virtual bool isValid() const;
    
    /**
     * Utility. Tell if the data type occupies any space (size concept makes sense).
     * Most data types are sized, with the exception of the unknown data type and void.
     */
    virtual bool isSized() const;
    
    /* Utility. Tell if this data type is the unknown data type. */
    virtual bool isUnknown() const;
    
    /* Utility. Tell if this data type is the 'void' data type. */
    virtual bool isVoid() const;
    
    /* Utility. Tell if this data type is the 'bool' data type. */
    virtual bool isBool() const;
    
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
