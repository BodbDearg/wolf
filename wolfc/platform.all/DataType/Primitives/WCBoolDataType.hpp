#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Boolean data type. Bools are always stored as 1 byte. */
class BoolDataType : public DataType {
public:
    virtual const std::string & name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual bool isBool() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;
};

WC_END_NAMESPACE
