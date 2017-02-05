#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Data type that represents unevaluated data type.
 * This is used for when we don't know the data type for something, because it has to be determined 
 * later through semantic analysis or similar levels of processing.
 */
class UnevalDataType final : public DataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isSized() const override;
};

WC_END_NAMESPACE
