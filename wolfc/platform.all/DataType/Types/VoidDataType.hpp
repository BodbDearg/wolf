//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Similar to 'unknown' in many ways but this type is exposed to the user. A data type that represents no
 * data type, or no data.
 */
class VoidDataType final : public DataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isSized() const override;
};

WC_END_NAMESPACE
