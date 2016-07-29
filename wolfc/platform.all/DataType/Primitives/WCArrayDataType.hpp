#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * A data type that represents an array.
 */
class ArrayDataType : public DataType {
public:
    ArrayDataType(DataType & innerType, size_t size, CodegenCtx & cgCtx);
    
    virtual ~ArrayDataType() override;
    
    virtual const std::string & name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual llvm::Type * llvmType(CodegenCtx & cgCtx) const override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;

    DataType &      mInnerType;
    size_t          mSize;
    std::string     mName;
    llvm::Type *    mLLVMType = nullptr;
};

WC_END_NAMESPACE
