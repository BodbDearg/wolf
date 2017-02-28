//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/* A data type that represents a pointer to a certain type. */
class PtrDataType final : public DataType {
public:
    PtrDataType(const DataType & pointedToType, bool isNullable);
    
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isPtr() const override;

    /* The type this pointer points to */
    const DataType & mPointedToType;
    
    /* True if the pointer type is allowed to have null values */
    bool mIsNullable;
    
    /* Name for the data type */
    std::string mName;
};

WC_END_NAMESPACE
