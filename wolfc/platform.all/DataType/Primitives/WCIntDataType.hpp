#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Signed integer data type. */
class IntDataType : public DataType {
public:
    virtual const std::string & name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual llvm::Type * llvmType(CodegenCtx & cgCtx) const override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;
};

WC_END_NAMESPACE
