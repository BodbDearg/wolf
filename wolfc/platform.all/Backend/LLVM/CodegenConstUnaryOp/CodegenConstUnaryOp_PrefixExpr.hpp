//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "CodegenBasicConstUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenConstPlusUnaryOp : public CodegenBasicConstUnaryOp {
public:
    CodegenConstPlusUnaryOp(ConstCodegen & cg,
                            const AST::ASTNode & expr);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenConstMinusUnaryOp : public CodegenBasicConstUnaryOp {
public:
    CodegenConstMinusUnaryOp(ConstCodegen & cg,
                             const AST::ASTNode & expr);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenConstPtrDenullUnaryOp : public CodegenBasicConstUnaryOp {
public:
    CodegenConstPtrDenullUnaryOp(ConstCodegen & cg,
                                 const AST::ASTNode & expr);
    
    virtual void codegen() override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
