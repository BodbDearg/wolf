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
    
    virtual llvm::AllocaInst * codegenAlloca(CodegenCtx & cgCtx,
                                             ASTNode & callingNode,
                                             const std::string & instLabel) override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & valToPrint) override;

    DataType &      mInnerType;
    size_t          mSize;
    std::string     mName;
    
protected:
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
};

WC_END_NAMESPACE
