#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

class ArrayDataType;
class BoolDataType;
class Int64DataType;
class StrDataType;
class UnknownArrayDataType;
class UnknownDataType;
class VoidDataType;

/* Interface for a visitor to data types */
class DataTypeVisitor {
public:
    virtual ~DataTypeVisitor() = default;
    
    virtual void visit(const ArrayDataType & dataType) = 0;
    virtual void visit(const BoolDataType & dataType) = 0;
    virtual void visit(const Int64DataType & dataType) = 0;
    virtual void visit(const StrDataType & dataType) = 0;
    virtual void visit(const UnknownArrayDataType & dataType) = 0;
    virtual void visit(const UnknownDataType & dataType) = 0;
    virtual void visit(const VoidDataType & dataType) = 0;
};

WC_END_NAMESPACE
