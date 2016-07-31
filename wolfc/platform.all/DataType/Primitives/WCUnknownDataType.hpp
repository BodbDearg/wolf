#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Data type that represents an unknown type. Cannot be declared by the user. Used internally by the
 * compiler in the case of an error.
 */
class UnknownDataType : public DataType {
public:
    virtual const std::string & name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual bool isValid() const override;
    
    virtual bool isSized() const override;
    
    virtual bool isUnknown() const override;
    
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    virtual llvm::AllocaInst * codegenAlloca(CodegenCtx & cgCtx,
                                             ASTNode & callingNode,
                                             const std::string & instLabel) override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;
};

WC_END_NAMESPACE
