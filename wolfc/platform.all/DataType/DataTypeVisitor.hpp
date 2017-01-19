#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

class ArrayDataType;
class BoolDataType;
class Int64DataType;
class StrDataType;
class UnknownDataType;
class VoidDataType;

/* Interface for a visitor to data types */
class DataTypeVisitor {
public:
    virtual ~DataTypeVisitor() = default;
    
    virtual void visit(const ArrayDataType & node) = 0;
    virtual void visit(const BoolDataType & node) = 0;
    virtual void visit(const Int64DataType & node) = 0;
    virtual void visit(const StrDataType & node) = 0;
    virtual void visit(const UnknownDataType & node) = 0;
    virtual void visit(const VoidDataType & node) = 0;
};

WC_END_NAMESPACE
