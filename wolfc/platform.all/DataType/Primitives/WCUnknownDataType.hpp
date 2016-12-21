#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Data type that represents an unknown type. Cannot be declared by the user. Used internally by the
 * compiler in the case of an error.
 */
class UnknownDataType final : public DataType {
public:
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
    virtual bool isSized() const override;
    
    virtual llvm::AllocaInst * codegenAlloca(CodegenCtx & cgCtx,
                                             ASTNode & callingNode,
                                             const std::string & instLabel) override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & valToPrint) override;
    
protected:
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
};

WC_END_NAMESPACE
