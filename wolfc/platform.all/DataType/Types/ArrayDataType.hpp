//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/* A data type that represents an array. */
class ArrayDataType final : public DataType {
public:
    ArrayDataType(const DataType & elemType, size_t size);
    
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isArray() const override;
    virtual bool requiresStorage() const override;

    const DataType &    mElemType;
    size_t              mSize;
    std::string         mName;
};

WC_END_NAMESPACE
