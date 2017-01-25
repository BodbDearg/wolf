#include "WCArrayBadSizeDataType.hpp"

#include "AST/Nodes/WCPrintStmnt.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

ArrayBadSizeDataType::ArrayBadSizeDataType(const AST::ASTNode & declaringNode,
                                           const DataType & elemType,
                                           AST::AssignExpr & sizeExpr)
:
    ArrayUnevalSizeDataType(declaringNode, elemType, sizeExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void ArrayBadSizeDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId ArrayBadSizeDataType::getTypeId() const {
    return DataTypeId::kArrayBadSize;
}

bool ArrayBadSizeDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Check that dynamic types match:
    auto otherArrayType = dynamic_cast<const ArrayBadSizeDataType*>(&other);
    WC_GUARD(otherArrayType, false);
    
    // Only consider the types equal if the AST node for the expression is the same.
    // Even the same expression evaluated at different points could mean different things.
    WC_GUARD(&mSizeExpr == &otherArrayType->mSizeExpr, false);
    
    // Lastly see if the element types match:
    return mElemType.equals(otherArrayType->mElemType);
}

WC_END_NAMESPACE
