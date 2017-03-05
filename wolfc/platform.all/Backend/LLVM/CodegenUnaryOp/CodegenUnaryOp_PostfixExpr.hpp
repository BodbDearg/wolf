//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "CodegenBasicUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenIncOrDecUnaryOp : public CodegenBasicUnaryOp {
public:
    CodegenIncOrDecUnaryOp(Codegen & cg,
                           const AST::ASTNode & expr,
                           const char * opSymbol,
                           const char * opName);
    
protected:
    /**
     * Utility function: checks that the given pointer data type is able to be used for pointer 
     * arithmetic and issues an error if it is not usable in pointer arithmetic. We can't do pointer 
     * arithmetic on a pointer to type 'any' for instance becasue we don't the size of the pointed 
     * to element.
     *
     * Returns false if the check fails and the pointer type can't be used for pointer arithmetic.
     */
    bool verifyPtrTypeOkForArithmetic(const PtrDataType & ptrType);
};

class CodegenIncUnaryOp final : public CodegenIncOrDecUnaryOp {
public:
    CodegenIncUnaryOp(Codegen & cg, const AST::ASTNode & expr);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const PtrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenDecUnaryOp final : public CodegenIncOrDecUnaryOp {
public:
    CodegenDecUnaryOp(Codegen & cg, const AST::ASTNode & expr);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const PtrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
