#pragma once

#include "WCDataTypeId.hpp"
#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

class DataTypeVisitor;

/* Base class for a data type */
class DataType {
public:
    virtual ~DataType();
    
    /* Visitor pattern accept function */
    virtual void accept(DataTypeVisitor & visitor) const = 0;
    
    /* Get the id of the data type */
    virtual DataTypeId getTypeId() const = 0;
    
    /* Give the name of the datatype, e.g 'int32' */
    virtual const std::string & name() const = 0;
    
    /* Tells if this data type matches another. */
    virtual bool equals(const DataType & other) const = 0;
    
    /**
     * Tells if the data type is valid. 
     * Returns 'true' for all data types except unknown data types.
     */
    virtual bool isValid() const;
    
    /**
     * Utility. Tell if the data type occupies any space (size concept makes sense).
     * Most data types are sized, with the exception of the unknown data type and void.
     */
    virtual bool isSized() const;
    
    /**
     * Utility. Tell if this data type is an unknown form of data type such 
     * as the 'Unknown' data type itself or an array data type with an 
     * unknown/unevaluated size.
     *
     * Note: this can also be true for an array data type where the size is known, 
     * if the inner type is actually unknown.
     */
    virtual bool isUnknown() const;
    
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
    
    /**
     * Utility. Tell if this data type is an array data type.
     * Returns true if the array is either an array with a known type
     * or an unknown type.
     */
    virtual bool isArray() const;
    
    /**
     * If true then the data type requires a storage area to hold it's data since it is
     * not a simple primitive that can be held in a register. This will be true for arrays, 
     * structs and other aggregate types.
     */
    virtual bool requiresStorage() const;
    
#warning FIXME - Codegen
#if 0
    /**
     * Generate code to allocate space for this type on the stack.
     * Returns the instruction for that when done.
     */
    virtual llvm::AllocaInst * codegenAlloca(CodegenCtx & cgCtx,
                                             AST::ASTNode & callingNode,
                                             const std::string & instLabel);
    
    /* Codegen a basic cast to the given type */
    virtual llvm::Value * codegenCastTo(CodegenCtx & cgCtx,
                                        AST::ASTNode & callingNode,
                                        llvm::Value & valToCast,
                                        DataType & castToTy);
#endif
    
#warning FIXME - Codegen
#if 0
    /**
     * Codegen various runtime unary operations.
     * The default impl issues an 'unsupported op' compile error.
     */
    #define WC_DATA_TYPE_DECL_CODEGEN_UNARY_OP_FUNC(OpName)\
        virtual llvm::Value * codegen ## OpName ## Op(CodegenCtx & cgCtx,\
                                                      AST::ASTNode & callingNode,\
                                                      llvm::Value & val);
    
    WC_DATA_TYPE_DECL_CODEGEN_UNARY_OP_FUNC(BNot)
    WC_DATA_TYPE_DECL_CODEGEN_UNARY_OP_FUNC(Plus)
    WC_DATA_TYPE_DECL_CODEGEN_UNARY_OP_FUNC(Minus)
    WC_DATA_TYPE_DECL_CODEGEN_UNARY_OP_FUNC(Inc)
    WC_DATA_TYPE_DECL_CODEGEN_UNARY_OP_FUNC(Dec)
    
    /**
     * Codegen various compile time binary operations: the two values must to be of the same type.
     * The default impl issues an 'unsupported op' compile error.
     */
    #define WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(OpName)\
        virtual llvm::Constant * codegenConst ## OpName ## Op(AST::ASTNode & callingNode,\
                                                              llvm::Constant & leftVal,\
                                                              DataType & rightTy,\
                                                              llvm::Constant & rightVal);
    
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(CmpEQ)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(CmpNE)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(CmpLT)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(CmpLE)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(CmpGT)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(CmpGE)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(Add)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(Sub)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(BOr)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(BXOr)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(Mul)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(Div)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(Mod)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(BAnd)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(LShift)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(ARShift)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_BINARY_OP_FUNC(LRShift)
    
    /**
     * Codegen various compile time unary operations.
     * The default impl issues an 'unsupported op' compile error.
     */
    #define WC_DATA_TYPE_DECL_CODEGEN_CONST_UNARY_OP_FUNC(OpName)\
        virtual llvm::Constant * codegenConst ## OpName ## Op(AST::ASTNode & callingNode,\
                                                              llvm::Constant & val);
    
    WC_DATA_TYPE_DECL_CODEGEN_CONST_UNARY_OP_FUNC(BNot)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_UNARY_OP_FUNC(Plus)
    WC_DATA_TYPE_DECL_CODEGEN_CONST_UNARY_OP_FUNC(Minus)

    /**
     * Issues a compile error that a binary operator is not supported.
     */
    void issueUnaryOpNotAvailableCompileError(AST::ASTNode & callingNode,
                                              const char * opSymbol,
                                              const char * opName) const;
#endif
};

WC_END_NAMESPACE
