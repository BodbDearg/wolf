#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * String data type.
 * TODO: will probably change once we have a string class and once we support pointers to chars.
 */
class StrDataType : public DataType {
public:
    virtual const char * name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;
};

WC_END_NAMESPACE
