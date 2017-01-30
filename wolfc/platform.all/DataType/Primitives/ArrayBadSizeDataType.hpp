#pragma once

#include "ArrayUnevalSizeDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * A data type that represents an array where the size element is bad, either because
 * it cannot be evaluated (due to a compile error) or because the size is invalid, 
 * e.g zero or negative sized.
 *
 * This data type is generated when evaluating a 'ArrayUnevalSizeDataType' fails.
 */
class ArrayBadSizeDataType final : public ArrayUnevalSizeDataType {
public:
    ArrayBadSizeDataType(const AST::ASTNode & declaringNode,
                         const DataType & elemType,
                         AST::AssignExpr & sizeExpr);
    
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual bool equals(const DataType & other) const override;
};

WC_END_NAMESPACE
