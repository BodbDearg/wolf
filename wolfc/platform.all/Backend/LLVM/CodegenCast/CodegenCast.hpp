//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../Value.hpp"
#include "DataType/DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class Codegen;

/* Code generates a runtime cast from a value of one type to another type */
class CodegenCast final : public DataTypeVisitor {
public:
    CodegenCast(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT);
    
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
    virtual void visit(const InvalidDataType & dataType) override;
    virtual void visit(const PtrDataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
private:
    Codegen &           mCG;
    Value               mFromVal;
    CompiledDataType    mToTypeCDT;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
