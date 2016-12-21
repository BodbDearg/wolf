#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all integer data types */
class GenericIntDataType : public DataType {
public:
    virtual bool isInteger() const final override;
    
    virtual llvm::Value * codegenAddOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenSubOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenBOrOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenBXOrOp(CodegenCtx & cgCtx,
                                        ASTNode & callingNode,
                                        llvm::Value & leftVal,
                                        DataType & rightTy,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Constant * codegenConstAddOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstSubOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstBOrOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstBXOrOp(ASTNode & callingNode,
                                                llvm::Constant & leftVal,
                                                DataType & rightTy,
                                                llvm::Constant & rightVal) override;
};

WC_END_NAMESPACE
