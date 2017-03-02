//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * A special data type that represents the type of the 'null' literal.
 * Not intended to be used actual variables but for automatic type conversions 
 * from the 'null' literal to other pointer types.
 */
class NullptrDataType final : public DataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isSized() const override;
};

WC_END_NAMESPACE
