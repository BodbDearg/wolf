#include "ArrayDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

ArrayDataType::ArrayDataType(const DataType & elemType, size_t size) :
    mElemType(elemType),
    mSize(size)
{
    // TODO: allocate the name elsewhere?
    // Makeup the name:
    mName.reserve(mElemType.name().size() + 16);
    mName += mElemType.name();
    mName += '[';
    
    {
        char buffer[32];
        std::snprintf(buffer, 32, "%zu", mSize);
        mName += buffer;
    }
    
    mName += ']';
}

void ArrayDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId ArrayDataType::getTypeId() const {
    return DataTypeId::kArray;
}

const std::string & ArrayDataType::name() const {
    return mName;
}

bool ArrayDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Check that dynamic types match:
    const ArrayDataType * otherType = dynamic_cast<const ArrayDataType*>(&other);
    WC_GUARD(otherType, false);
    
    // See if array sizes match:
    WC_GUARD(mSize == otherType->mSize, false);
    
    // Lastly see if the element types match:
    return mElemType.equals(otherType->mElemType);
}

bool ArrayDataType::isValid() const {
    // Only valid if the element type is valid!
    return mElemType.isValid();
}

bool ArrayDataType::isArray() const {
    return true;
}

bool ArrayDataType::requiresStorage() const {
    return true;
}

WC_END_NAMESPACE
