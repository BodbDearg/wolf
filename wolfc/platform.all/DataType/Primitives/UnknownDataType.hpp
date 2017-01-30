#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Data type that represents an unknown scalar type. 
 * Cannot be declared by the user, used internally by the compiler.
 */
class UnknownDataType final : public DataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isSized() const override;
    virtual bool isUnknown() const override;
};

WC_END_NAMESPACE
