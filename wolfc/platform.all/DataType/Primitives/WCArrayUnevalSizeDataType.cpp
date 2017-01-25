#include "WCArrayUnevalSizeDataType.hpp"

#include "AST/Nodes/WCPrintStmnt.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

ArrayUnevalSizeDataType::ArrayUnevalSizeDataType(const AST::ASTNode & declaringNode,
                                                 const DataType & elemType,
                                                 AST::AssignExpr & sizeExpr)
:
    mDeclaringNode(declaringNode),
    mElemType(elemType),
    mSizeExpr(sizeExpr)
{
    // TODO: allocate the name elsewhere?
    // Makeup the name:
    mName.reserve(mElemType.name().size() + 16);
    mName += "[?]";    
    mName += mElemType.name();
}

ArrayUnevalSizeDataType::~ArrayUnevalSizeDataType() {
    // Defined here so llvm::Type can be forward referenced in the header.
    WC_EMPTY_FUNC_BODY();
}

void ArrayUnevalSizeDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId ArrayUnevalSizeDataType::getTypeId() const {
    return DataTypeId::kArrayUnevalSize;
}

const std::string & ArrayUnevalSizeDataType::name() const {
    return mName;
}

bool ArrayUnevalSizeDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Check that dynamic types match:
    auto otherArrayType = dynamic_cast<const ArrayUnevalSizeDataType*>(&other);
    WC_GUARD(otherArrayType, false);
    
    // Only consider the types equal if the AST node for the expression is the same.
    // Even the same expression evaluated at different points could mean different things.
    WC_GUARD(&mSizeExpr == &otherArrayType->mSizeExpr, false);
    
    // Lastly see if the element types match:
    return mElemType.equals(otherArrayType->mElemType);
}

bool ArrayUnevalSizeDataType::isValid() const {
    return false;
}

bool ArrayUnevalSizeDataType::isUnknown() const {
    // Only known if the element type is not unknown
    return mElemType.isUnknown();
}

bool ArrayUnevalSizeDataType::isArray() const {
    return true;
}

bool ArrayUnevalSizeDataType::requiresStorage() const {
    return true;
}

WC_END_NAMESPACE
