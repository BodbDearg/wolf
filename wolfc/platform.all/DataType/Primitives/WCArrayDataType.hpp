#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* A data type that represents an array. */
class ArrayDataType final : public DataType {
public:
    ArrayDataType(DataType & innerType, size_t size);
    virtual ~ArrayDataType() override;
    
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool requiresStorage() const override;

    DataType &      mInnerType;
    size_t          mSize;
    std::string     mName;
    
protected:
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) override;
#endif
};

WC_END_NAMESPACE
