//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../DataType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/* A data type that represents a function. */
class FuncDataType final : public DataType {
public:
    FuncDataType(const DataType & returnType, std::vector<const DataType*> && argTypes);
    
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isFunc() const override;
    
    /* Tells if all args and return types for the function are valid */
    bool areAllArgsAndReturnTypesValid() const;
    
    /* The return type for the function */
    const DataType & mReturnType;
    
    /* The argument types for the functions */
    std::vector<const DataType*> mArgTypes;
    
private:
    /* The name for the function data type */
    std::string mName;
};

WC_END_NAMESPACE
