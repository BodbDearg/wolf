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
    
    virtual llvm::Value * codegenMulOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenBAndOp(CodegenCtx & cgCtx,
                                        ASTNode & callingNode,
                                        llvm::Value & leftVal,
                                        DataType & rightTy,
                                        llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenLShiftOp(CodegenCtx & cgCtx,
                                          ASTNode & callingNode,
                                          llvm::Value & leftVal,
                                          DataType & rightTy,
                                          llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenLRShiftOp(CodegenCtx & cgCtx,
                                           ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenIncOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & val) override;
    
    virtual llvm::Value * codegenDecOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & val) override;
    
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
    
    virtual llvm::Constant * codegenConstMulOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstBAndOp(ASTNode & callingNode,
                                                llvm::Constant & leftVal,
                                                DataType & rightTy,
                                                llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstLShiftOp(ASTNode & callingNode,
                                                  llvm::Constant & leftVal,
                                                  DataType & rightTy,
                                                  llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstLRShiftOp(ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal) override;
};

WC_END_NAMESPACE
