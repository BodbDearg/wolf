#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Boolean data type. Bools are always stored as 1 byte. */
class BoolDataType final : public DataType {
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
    
    virtual llvm::Value * codegenCmpEQOp(CodegenCtx & cgCtx,
                                         AST::ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpNEOp(CodegenCtx & cgCtx,
                                         AST::ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpEQOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpNEOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
#endif
    
protected:
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) override;
#endif
};

WC_END_NAMESPACE
