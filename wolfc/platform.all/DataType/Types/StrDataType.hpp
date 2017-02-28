#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * String data type.
 * TODO: will probably change once we have a string class and once we support pointers to chars.
 */
class StrDataType final : public DataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
};

WC_END_NAMESPACE
