#pragma once

#include "../DataType.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class AssignExpr;
    class ASTNode;
}

/**
 * A data type that represents an array where the size element is unknown/unevaluated.
 * At compile time the assign expression for the array size must be evaluated to get
 * the concrete array type.
 */
class ArrayUnevalSizeDataType : public DataType {
public:
    ArrayUnevalSizeDataType(const AST::ASTNode & declaringNode,
                            const DataType & elemType,
                            AST::AssignExpr & sizeExpr);
    
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const final override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const final override;
    virtual bool isArray() const final override;
    virtual bool requiresStorage() const final override;

    /* The node that declared this type */
    const AST::ASTNode & mDeclaringNode;
    
    /* The element type for the array */
    const DataType & mElemType;
    
    /* The size expression determining the size of the array */
    AST::AssignExpr & mSizeExpr;
    
    /* The name of the array data type */
    std::string mName;
};

WC_END_NAMESPACE
