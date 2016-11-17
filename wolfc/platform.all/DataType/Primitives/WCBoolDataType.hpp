#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Boolean data type. Bools are always stored as 1 byte. */
class BoolDataType final : public DataType {
public:
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isBool() const override;
    
    virtual llvm::AllocaInst * codegenAlloca(CodegenCtx & cgCtx,
                                             ASTNode & callingNode,
                                             const std::string & instLabel) override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;
    
protected:
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
};

WC_END_NAMESPACE
