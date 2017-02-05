#include "FuncUnevalDataType.hpp"

#include "../DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

FuncUnevalDataType::FuncUnevalDataType(const AST::ASTNode & declaringNode,
                                       const DataType & returnType,
                                       std::vector<const DataType*> && argTypes)
:
    mDeclaringNode(declaringNode),
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

void FuncUnevalDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId FuncUnevalDataType::getTypeId() const {
    return DataTypeId::kFuncUneval;
}

const std::string & FuncUnevalDataType::name() const {
    return mName;
}

bool FuncUnevalDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Check that dynamic types match:
    const FuncUnevalDataType * otherType = dynamic_cast<const FuncUnevalDataType*>(&other);
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

bool FuncUnevalDataType::isValid() const {
    // Not valid because it is unevaluated
    return false;
}

bool FuncUnevalDataType::isFunc() const {
    return true;
}

WC_END_NAMESPACE
