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
    virtual bool isInteger() const;
    
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
                                             const std::string & instLabel);
    
    /**
     * Generate the code for a print statement for this type. The codegen context, printf function prototype, 
     * parent print statement and the value to be printed are passed in as params.
     * The default implementation of this fails with an 'unsupported op' error.
     */
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   ASTNode & callingNode,
                                   llvm::Constant & printfFn,
                                   llvm::Value & valToPrint);
    
    /* Codegen a basic cast to the given type */
    virtual llvm::Value * codegenCastTo(CodegenCtx & cgCtx,
                                        ASTNode & callingNode,
                                        llvm::Value & valToCast,
                                        DataType & castToTy);
    
    /**
     * Codegen various runtime binary operations: the two values must to be of the same type.
     * The default impl issues an 'unsupported op' compile error.
     */
    virtual llvm::Value * codegenAddOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal);
    
    virtual llvm::Value * codegenSubOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal);
    
    virtual llvm::Value * codegenBOrOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal);
    
    virtual llvm::Value * codegenBXOrOp(CodegenCtx & cgCtx,
                                        ASTNode & callingNode,
                                        llvm::Value & leftVal,
                                        DataType & rightTy,
                                        llvm::Value & rightVal);
    
    virtual llvm::Value * codegenMulOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal);
    
    virtual llvm::Value * codegenDivOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal);
    
    virtual llvm::Value * codegenModOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal);
    
    virtual llvm::Value * codegenBAndOp(CodegenCtx & cgCtx,
                                        ASTNode & callingNode,
                                        llvm::Value & leftVal,
                                        DataType & rightTy,
                                        llvm::Value & rightVal);
    
    virtual llvm::Value * codegenLShiftOp(CodegenCtx & cgCtx,
                                          ASTNode & callingNode,
                                          llvm::Value & leftVal,
                                          DataType & rightTy,
                                          llvm::Value & rightVal);
    
    virtual llvm::Value * codegenARShiftOp(CodegenCtx & cgCtx,
                                           ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal);
    
    virtual llvm::Value * codegenLRShiftOp(CodegenCtx & cgCtx,
                                           ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal);
    
    /**
     * Codegen various compile time binary operations: the two values must to be of the same type.
     * The default impl issues an 'unsupported op' compile error.
     */
    virtual llvm::Constant * codegenConstAddOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstSubOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstBOrOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstBXOrOp(ASTNode & callingNode,
                                                llvm::Constant & leftVal,
                                                DataType & rightTy,
                                                llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstMulOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal);

    virtual llvm::Constant * codegenConstDivOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstModOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstBAndOp(ASTNode & callingNode,
                                                llvm::Constant & leftVal,
                                                DataType & rightTy,
                                                llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstLShiftOp(ASTNode & callingNode,
                                                  llvm::Constant & leftVal,
                                                  DataType & rightTy,
                                                  llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstARShiftOp(ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal);
    
    virtual llvm::Constant * codegenConstLRShiftOp(ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal);
    
    /**
     * Check that the LLVM type for this data type is defined and issue a compile error if not.
     * Returns false if the LLVM type is not defined and true otherwise.
     */
    bool compileCheckLLVMTypeDefined(ASTNode & callingNode);
    
    /**
     * Does a compile check that the right expression type in a binary operator matches
     * exactly this data type. Issues a compile error and returns false if the check fails.
     */
    bool compileCheckBinaryOpTypesMatch(ASTNode & callingNode,
                                        const char * opSymbol,
                                        const char * opName,
                                        DataType & rightExprType);
    
    /**
     * Issues a compile error that an operator is not supported between this type and 
     * an expression of a given type.
     */
    void issueOpNotAvailableCompileError(ASTNode & callingNode,
                                         const char * opSymbol,
                                         const char * opName,
                                         DataType & rightExprType) const;
    
    /**
     * The llvm type for the data type. This is filled in during code generation.
     * Note: The unknown data type will not use anything for this field...
     */
    llvm::Type * mLLVMType = nullptr;
    
protected:
    static const char * const kOpSymbol_Add;
    static const char * const kOpSymbol_Sub;
    static const char * const kOpSymbol_BOr;
    static const char * const kOpSymbol_BXOr;
    static const char * const kOpSymbol_Mul;
    static const char * const kOpSymbol_Div;
    static const char * const kOpSymbol_Mod;
    static const char * const kOpSymbol_BAnd;
    static const char * const kOpSymbol_LShift;
    static const char * const kOpSymbol_ARShift;
    static const char * const kOpSymbol_LRShift;
    
    static const char * const kOpName_Add;
    static const char * const kOpName_Sub;
    static const char * const kOpName_BOr;
    static const char * const kOpName_BXOr;
    static const char * const kOpName_Mul;
    static const char * const kOpName_Div;
    static const char * const kOpName_Mod;
    static const char * const kOpName_BAnd;
    static const char * const kOpName_LShift;
    static const char * const kOpName_ARShift;
    static const char * const kOpName_LRShift;
    
    /**
     * Run code generation for this data type.
     * Generates the llvm type for the data type and returns true if generation was successful.
     */
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) = 0;
    
    /* Issue a generic compile error for failing to generate the LLVM type for this data type. */
    void issueGenericCodegenLLVMTypeError(ASTNode & callingNode) const;
};

WC_END_NAMESPACE
