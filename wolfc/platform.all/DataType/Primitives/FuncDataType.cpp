#include "FuncDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

FuncDataType::FuncDataType(const DataType & returnType, std::vector<const DataType*> && argTypes)
:
    mReturnType(returnType),
    mArgTypes(argTypes)
{
    // TODO: allocate the name elsewhere?
    // Estimate storage required for the name.
    size_t sizeEstimate = 16 + (mArgTypes.size() * 2) + mReturnType.name().length();
    
    for (const DataType * type : mArgTypes) {
        sizeEstimate += type->name().length();
    }
    
    sizeEstimate += returnType.name().length();
    
    // Makeup the name:
    mName.reserve(sizeEstimate);
    mName += "func (";
    
    for (size_t i = 0; i < mArgTypes.size(); ++i) {
        const DataType * type = mArgTypes[i];
        mName += type->name();
        
        if (i + 1 < mArgTypes.size()) {
            mName += ", ";
        }
    }
    
    mName += ')';
    
    if (!mReturnType.isVoid()) {
        mName += " -> ";
        mName += mReturnType.name();
    }
}

void FuncDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId FuncDataType::getTypeId() const {
    return DataTypeId::kFunc;
}

const std::string & FuncDataType::name() const {
    return mName;
}

bool FuncDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Check that dynamic types match:
    const FuncDataType * otherType = dynamic_cast<const FuncDataType*>(&other);
    WC_GUARD(otherType, false);
    
    // Argument count must match:
    if (mArgTypes.size() != otherType->mArgTypes.size()) {
        return false;
    }
    
    // Argument types must match:
    for (size_t i = 0; i < mArgTypes.size(); ++i) {
        const DataType * type1 = mArgTypes[i];
        const DataType * type2 = otherType->mArgTypes[i];
        
        if (!type1->equals(*type2)) {
            return false;
        }
    }
    
    // Return types must match:
    return mReturnType.equals(otherType->mReturnType);
}

bool FuncDataType::isValid() const {
    // All arg types must be valid
    for (const DataType * type : mArgTypes) {
        if (!type->isValid()) {
            return false;
        }
    }
    
    // Return type must be valid
    return mReturnType.isValid();
}

bool FuncDataType::isUnknown() const {
    // See if any arg types are unknown
    for (const DataType * type : mArgTypes) {
        if (type->isUnknown()) {
            return true;
        }
    }
    
    // Tell if return type is unknown
    return mReturnType.isUnknown();
}

WC_END_NAMESPACE
