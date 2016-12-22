#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Boolean data type. Bools are always stored as 1 byte. */
class BoolDataType final : public DataType {
public:
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   ASTNode & callingNode,
                                   llvm::Constant & printfFn,
                                   llvm::Value & valToPrint) override;
    
    virtual llvm::Value * codegenCmpEQOp(CodegenCtx & cgCtx,
                                         ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpNEOp(CodegenCtx & cgCtx,
                                         ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpEQOp(ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpNEOp(ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
protected:
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) override;
};

WC_END_NAMESPACE
