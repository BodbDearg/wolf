#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Boolean data type. Bools are always stored as 1 byte. */
class BoolDataType : public DataType {
public:
    virtual const char * name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual bool isSized() const override;
    
    virtual llvm::Type * llvmType(CodegenCtx & cgCtx) const override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;
};

WC_END_NAMESPACE
