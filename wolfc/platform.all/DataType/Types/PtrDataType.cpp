//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "PtrDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

PtrDataType::PtrDataType(const DataType & pointedToType, bool isNullable) :
    mPointedToType(pointedToType),
    mIsNullable(isNullable)
{
    // TODO: allocate the name elsewhere?
    // Makeup the name:
    mName.reserve(mPointedToType.name().size() + 1);
    
    if (mIsNullable) {
        mName += '?';
    }
    else {
        mName += '^';
    }
    
    mName += mPointedToType.name();
}

void PtrDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId PtrDataType::getTypeId() const {
    return DataTypeId::kPtr;
}

const std::string & PtrDataType::name() const {
    return mName;
}

bool PtrDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Check that dynamic types match:
    const PtrDataType * otherType = dynamic_cast<const PtrDataType*>(&other);
    WC_GUARD(otherType, false);
    
    // Pointed to types must match
    return mPointedToType.equals(otherType->mPointedToType);
}

bool PtrDataType::isValid() const {
    // Special case: pointers to type 'any' are valid, even though the 'any' data type is
    // not a valid type on it's own. We can have pointers and references to 'any', but not
    // value types of type 'any':
    if (mPointedToType.isAny()) {
        return true;
    }
    
    // Only valid if the pointed to type is valid!
    return mPointedToType.isValid();
}

bool PtrDataType::isPtr() const {
    return true;
}

WC_END_NAMESPACE
