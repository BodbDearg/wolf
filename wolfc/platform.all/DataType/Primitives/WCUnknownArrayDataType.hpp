#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class AssignExpr;
    class ASTNode;
}

#warning TODO: rename to 'UnevalSize'
/**
 * A data type that represents an array where the size element is unknown/unevaluated.
 * At compile time the assign expression for the array size must be evaluated to get
 * the concrete array type.
 */
class UnknownArrayDataType final : public DataType {
public:
    UnknownArrayDataType(const AST::ASTNode & declaringNode,
                         const DataType & innerType,
                         AST::AssignExpr & sizeExpr);
    
    virtual ~UnknownArrayDataType() override;
    
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isUnknown() const override;
    virtual bool isArray() const override;
    virtual bool requiresStorage() const override;

    /* The node that declared this type */
    const AST::ASTNode & mDeclaringNode;
    
    /* The element type for hte array */
    const DataType & mInnerType;
    
    /* The size expression determining the size of the array */
    AST::AssignExpr & mSizeExpr;
    
    /* The name of the array data type */
    std::string mName;
};

WC_END_NAMESPACE
