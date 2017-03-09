//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../Constant.hpp"
#include "DataType/DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class ConstCodegen;

/* Helper class that creates a compile time cast from a value of one type to another type */
class CodegenConstCast final : public DataTypeVisitor {
public:
    CodegenConstCast(ConstCodegen & cg,
                     const Constant & fromConst,
                     const CompiledDataType & toTypeCDT);
    
    void codegen();
    
    virtual void visit(const AnyDataType & dataType) override;
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const FuncDataType & dataType) override;
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const NullptrDataType & dataType) override;
    virtual void visit(const PtrDataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
    virtual void visit(const UndefinedDataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
private:
    ConstCodegen &      mCG;
    Constant            mFromConst;
    CompiledDataType    mToTypeCDT;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
