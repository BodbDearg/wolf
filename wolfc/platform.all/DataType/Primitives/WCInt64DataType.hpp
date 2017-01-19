#pragma once

#include "WCGenericSignedIntDataType.hpp"

WC_BEGIN_NAMESPACE

/* Signed 64-bit integer data type. */
class Int64DataType final : public GenericSignedIntDataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   AST::ASTNode & callingNode,
                                   llvm::Constant & printfFn,
                                   llvm::Value & valToPrint) override;
#endif

protected:
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) override;
#endif
};

WC_END_NAMESPACE
