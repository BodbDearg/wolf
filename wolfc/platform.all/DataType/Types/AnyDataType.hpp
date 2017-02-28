//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * A special data type that represents 'any' kind of non void data type.
 * Used for pointers and references that can point to anything, and as the
 * data type for the 'null' pointer literal.
 *
 * In C++ the equivalent of this type would be 'void *'.
 *
 * Values of type 'any' are special in that they cannot be copied or accessed 
 * by value, you can only pass them around by pointer or reference.
 */
class AnyDataType final : public DataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isSized() const override;
};

WC_END_NAMESPACE
