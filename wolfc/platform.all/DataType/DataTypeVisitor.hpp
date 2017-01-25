#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

class ArrayBadSizeDataType;
class ArrayDataType;
class ArrayUnevalSizeDataType;
class BoolDataType;
class Int64DataType;
class StrDataType;
class UnknownDataType;
class VoidDataType;

/* Interface for a visitor to data types */
class DataTypeVisitor {
public:
    virtual ~DataTypeVisitor() = default;
    
    virtual void visit(const ArrayBadSizeDataType & dataType) = 0;
    virtual void visit(const ArrayDataType & dataType) = 0;
    virtual void visit(const ArrayUnevalSizeDataType & dataType) = 0;
    virtual void visit(const BoolDataType & dataType) = 0;
    virtual void visit(const Int64DataType & dataType) = 0;
    virtual void visit(const StrDataType & dataType) = 0;
    virtual void visit(const UnknownDataType & dataType) = 0;
    virtual void visit(const VoidDataType & dataType) = 0;
};

WC_END_NAMESPACE
