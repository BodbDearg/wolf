#pragma once

#include "WCDataTypeId.hpp"
#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class AllocaInst;
    class Constant;
    class Type;
    class Value;
}

WC_BEGIN_NAMESPACE

class ASTNode;
class PrintStmnt;
struct CodegenCtx;

/* Base class for a data type */
class DataType {
public:
    virtual ~DataType();
    
    /* Get the id of the data type */
    virtual DataTypeId getTypeId() const = 0;
    
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
    inline bool isUnknown() const {
        return getTypeId() == DataTypeId::kUnknown;
    }
    
    /* Utility. Tell if this data type is the 'void' data type. */
    inline bool isVoid() const {
        return getTypeId() == DataTypeId::kVoid;
    }
    
    /* Utility. Tell if this data type is the 'bool' data type. */
    inline bool isBool() const {
        return getTypeId() == DataTypeId::kBool;
    }
    
    /* Utility. Tell if this data type is an integer data type. */
    inline bool isInteger() const {
        return getTypeId() == DataTypeId::kInt;
    }
    
    /* Utility. Tell if this data type is an array data type. */
    inline bool isArray() const {
        return getTypeId() == DataTypeId::kArray;
    }
    
    /**
     * If true then the data type requires a storage area to hold it's data since it is
     * not a simple primitive that can be held in a register. This will be true for arrays, 
     * structs and other aggregate types.
     */
    virtual bool requiresStorage() const;
    
    /**
     * Run code generation for this data type (if required). If not required does nothing
     * Generates the llvm type for the data type and returns true if generation was successful.
     */
    bool codegenLLVMTypeIfRequired(CodegenCtx & cgCtx, ASTNode & callingNode);
    
    /**
     * Generate code to allocate space for this type on the stack.
     * Returns the instruction for that when done.
     */
    virtual llvm::AllocaInst * codegenAlloca(CodegenCtx & cgCtx,
                                             ASTNode & callingNode,
                                             const std::string & instLabel) = 0;
    
    /**
     * Generate the code for a print statement for this type. The codegen context, printf function prototype, 
     * parent print statement and the value to be printed are passed in as params.
     */
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const = 0;
    
    /* Codegen a basic cast to the given type */
    virtual llvm::Value * codegenCastTo(CodegenCtx & cgCtx,
                                        ASTNode & callingNode,
                                        llvm::Value & valueToCast,
                                        DataType & castToType);
    
    /**
     * Check that the LLVM type for this data type is defined and issue a compile error if not.
     * Returns false if the LLVM type is not defined and true otherwise.
     */
    bool compileCheckLLVMTypeDefined(ASTNode & callingNode);
    
    /**
     * The llvm type for the data type. This is filled in during code generation.
     * Note: The unknown data type will not use anything for this field...
     */
    llvm::Type * mLLVMType = nullptr;
    
protected:
    /**
     * Run code generation for this data type.
     * Generates the llvm type for the data type and returns true if generation was successful.
     */
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) = 0;
    
    /* Issue a generic compile error for failing to generate the LLVM type for this data type. */
    void issueGenericCodegenLLVMTypeError(ASTNode & callingNode);
};

WC_END_NAMESPACE
