#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Signed integer data type. */
class IntDataType : public DataType {
public:
    virtual const char * name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual llvm::Value * genPrintStmntCode(const CodegenCtx & cgCtx,
                                            const PrintStmnt & parentPrintStmnt,
                                            llvm::Constant & printfFn,
                                            llvm::Value & value) const override;
};

WC_END_NAMESPACE
