#pragma once

#include "WCMacros.hpp"
#include <string>

namespace llvm {
    class Constant;
    class Type;
    class Value;
}

WC_BEGIN_NAMESPACE

class ASTNode;
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
     * Run code generation for this data type.
     * Generates the llvm type for the data type and returns true if generation was successful.
     */
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) = 0;
    
    /**
     * Generate the code for a print statement for this type. The codegen context, printf function prototype, 
     * parent print statement and the value to be printed are passed in as params.
     */
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const = 0;
    
    /**
     * The llvm type for the data type. This is filled in during code generation.
     * Note: The unknown data type will not use anything for this field...
     */
    llvm::Type * mLLVMType = nullptr;
};

WC_END_NAMESPACE
