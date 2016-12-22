#pragma once

#include "WCGenericSignedIntDataType.hpp"

WC_BEGIN_NAMESPACE

/* Signed 64-bit integer data type. */
class Int64DataType final : public GenericSignedIntDataType {
public:
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   ASTNode & callingNode,
                                   llvm::Constant & printfFn,
                                   llvm::Value & valToPrint) override;
    
protected:
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
};

WC_END_NAMESPACE
