#pragma once

#include "Macros.hpp"

WC_BEGIN_NAMESPACE

class ArrayBadSizeDataType;
class ArrayDataType;
class ArrayUnevalSizeDataType;
class BoolDataType;
class FuncDataType;
class Int64DataType;
class InvalidDataType;
class StrDataType;
class VoidDataType;

/* Interface for a visitor to data types */
class DataTypeVisitor {
public:
    virtual ~DataTypeVisitor() = default;
    
    virtual void visit(const ArrayBadSizeDataType & dataType) = 0;
    virtual void visit(const ArrayDataType & dataType) = 0;
    virtual void visit(const ArrayUnevalSizeDataType & dataType) = 0;
    virtual void visit(const BoolDataType & dataType) = 0;
    virtual void visit(const FuncDataType & dataType) = 0;
    virtual void visit(const Int64DataType & dataType) = 0;
    virtual void visit(const InvalidDataType & dataType) = 0;
    virtual void visit(const StrDataType & dataType) = 0;
    virtual void visit(const VoidDataType & dataType) = 0;
};

WC_END_NAMESPACE
