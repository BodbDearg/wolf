#pragma once

#include "WCGenericIntDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all signed integer data types */
class GenericSignedIntDataType : public GenericIntDataType {
public:
#warning FIXME - Codegen
#if 0
    virtual llvm::Value * codegenCmpLTOp(CodegenCtx & cgCtx,
                                         AST::ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpLEOp(CodegenCtx & cgCtx,
                                         AST::ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpGTOp(CodegenCtx & cgCtx,
                                         AST::ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;
    
    virtual llvm::Value * codegenCmpGEOp(CodegenCtx & cgCtx,
                                         AST::ASTNode & callingNode,
                                         llvm::Value & leftVal,
                                         DataType & rightTy,
                                         llvm::Value & rightVal) override;

    virtual llvm::Constant * codegenConstCmpLTOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpLEOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpGTOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpGEOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstDivOp(AST::ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstModOp(AST::ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstARShiftOp(AST::ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal) override;
#endif
};

WC_END_NAMESPACE
