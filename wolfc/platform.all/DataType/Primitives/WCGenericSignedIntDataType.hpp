#pragma once

#include "WCGenericIntDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all signed integer data types */
class GenericSignedIntDataType : public GenericIntDataType {
public:
    virtual llvm::Value * codegenCmpLTOp(CodegenCtx & cgCtx,
                                         ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpLEOp(CodegenCtx & cgCtx,
                                         ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpGTOp(CodegenCtx & cgCtx,
                                         ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpGEOp(CodegenCtx & cgCtx,
                                         ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenDivOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenModOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightTy,
                                       llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenARShiftOp(CodegenCtx & cgCtx,
                                           ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal) override;

    virtual llvm::Constant * codegenConstCmpLTOp(ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpLEOp(ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpGTOp(ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpGEOp(ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstDivOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstModOp(ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstARShiftOp(ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal) override;
};

WC_END_NAMESPACE
