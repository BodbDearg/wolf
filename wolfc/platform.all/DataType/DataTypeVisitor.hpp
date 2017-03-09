//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Macros.hpp"

WC_BEGIN_NAMESPACE

class AnyDataType;
class ArrayDataType;
class BoolDataType;
class FuncDataType;
class Int128DataType;
class Int16DataType;
class Int32DataType;
class Int64DataType;
class Int8DataType;
class NullptrDataType;
class PtrDataType;
class StrDataType;
class UInt128DataType;
class UInt16DataType;
class UInt32DataType;
class UInt64DataType;
class UInt8DataType;
class UndefinedDataType;
class VoidDataType;

/* Interface for a visitor to data types */
class DataTypeVisitor {
public:
    virtual ~DataTypeVisitor() = default;
    
    virtual void visit(const AnyDataType & dataType) = 0;
    virtual void visit(const ArrayDataType & dataType) = 0;
    virtual void visit(const BoolDataType & dataType) = 0;
    virtual void visit(const FuncDataType & dataType) = 0;
    virtual void visit(const Int128DataType & dataType) = 0;
    virtual void visit(const Int16DataType & dataType) = 0;
    virtual void visit(const Int32DataType & dataType) = 0;
    virtual void visit(const Int64DataType & dataType) = 0;
    virtual void visit(const Int8DataType & dataType) = 0;
    virtual void visit(const NullptrDataType & dataType) = 0;
    virtual void visit(const PtrDataType & dataType) = 0;
    virtual void visit(const StrDataType & dataType) = 0;
    virtual void visit(const UInt128DataType & dataType) = 0;
    virtual void visit(const UInt16DataType & dataType) = 0;
    virtual void visit(const UInt32DataType & dataType) = 0;
    virtual void visit(const UInt64DataType & dataType) = 0;
    virtual void visit(const UInt8DataType & dataType) = 0;
    virtual void visit(const UndefinedDataType & dataType) = 0;
    virtual void visit(const VoidDataType & dataType) = 0;
};

WC_END_NAMESPACE
