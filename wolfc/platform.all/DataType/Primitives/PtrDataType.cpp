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
    // Only valid if the pointed to type is valid!
    return mPointedToType.isValid();
}

bool PtrDataType::isPointer() const {
    return true;
}

WC_END_NAMESPACE
