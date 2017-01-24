#include "WCUnknownArrayDataType.hpp"

#include "AST/Nodes/WCPrintStmnt.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

UnknownArrayDataType::UnknownArrayDataType(const AST::ASTNode & declaringNode,
                                           const DataType & innerType,
                                           AST::AssignExpr & sizeExpr)
:
    mDeclaringNode(declaringNode),
    mInnerType(innerType),
    mSizeExpr(sizeExpr)
{
    // TODO: allocate the name elsewhere?
    // Makeup the name:
    mName.reserve(mInnerType.name().size() + 16);
    mName += mInnerType.name();
    mName += "[?]";
}

UnknownArrayDataType::~UnknownArrayDataType() {
    // Defined here so llvm::Type can be forward referenced in the header.
    WC_EMPTY_FUNC_BODY();
}

void UnknownArrayDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId UnknownArrayDataType::getTypeId() const {
    return DataTypeId::kUnknownArray;
}

const std::string & UnknownArrayDataType::name() const {
    return mName;
}

bool UnknownArrayDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Get the other type as an unknown array type,
    // if it is not an unknown array type then the types do not match:
    const UnknownArrayDataType * otherArrayType = dynamic_cast<const UnknownArrayDataType*>(&other);
    WC_GUARD(otherArrayType, false);
    
    // Only consider the types equal if the AST node for the expression is the same.
    // Even the same expression evaluated at different points could mean different things.
    WC_GUARD(&mSizeExpr == &otherArrayType->mSizeExpr, false);
    
    // Lastly see if the inner types match:
    return mInnerType.equals(otherArrayType->mInnerType);
}

bool UnknownArrayDataType::isValid() const {
    return false;
}

bool UnknownArrayDataType::isUnknown() const {
    return true;
}

bool UnknownArrayDataType::isArray() const {
    return true;
}

bool UnknownArrayDataType::requiresStorage() const {
    return true;
}

WC_END_NAMESPACE
