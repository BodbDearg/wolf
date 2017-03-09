//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "CodegenBasicUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenLNotUnaryOp final : public CodegenBasicUnaryOp {
public:
    CodegenLNotUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr,
                       bool storeResultInExpr);
    
    virtual void visit(const BoolDataType & dataType) override;
    
protected:
    virtual void doAnyImplicitTypeCastingRequiredForExprVal() override;
};

class CodegenBNotUnaryOp final : public CodegenBasicUnaryOp {
public:
    CodegenBNotUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr,
                       bool storeResultInExpr);
    
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

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
