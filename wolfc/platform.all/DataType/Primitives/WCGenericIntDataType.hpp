#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all integer data types */
class GenericIntDataType : public DataType {
public:
    virtual bool isInteger() const final override;
    
#warning FIXME - Codegen
#if 0
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

    virtual llvm::Value * codegenBNotOp(CodegenCtx & cgCtx,
                                        AST::ASTNode & callingNode,
                                        llvm::Value & val) override;
    
    virtual llvm::Value * codegenPlusOp(CodegenCtx & cgCtx,
                                        AST::ASTNode & callingNode,
                                        llvm::Value & val) override;
    
    virtual llvm::Value * codegenMinusOp(CodegenCtx & cgCtx,
                                         AST::ASTNode & callingNode,
                                         llvm::Value & val) override;
    
    virtual llvm::Value * codegenIncOp(CodegenCtx & cgCtx,
                                       AST::ASTNode & callingNode,
                                       llvm::Value & val) override;
    
    virtual llvm::Value * codegenDecOp(CodegenCtx & cgCtx,
                                       AST::ASTNode & callingNode,
                                       llvm::Value & val) override;

    virtual llvm::Constant * codegenConstCmpEQOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstCmpNEOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & leftVal,
                                                 DataType & rightTy,
                                                 llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstAddOp(AST::ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstSubOp(AST::ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstBOrOp(AST::ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstBXOrOp(AST::ASTNode & callingNode,
                                                llvm::Constant & leftVal,
                                                DataType & rightTy,
                                                llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstMulOp(AST::ASTNode & callingNode,
                                               llvm::Constant & leftVal,
                                               DataType & rightTy,
                                               llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstBAndOp(AST::ASTNode & callingNode,
                                                llvm::Constant & leftVal,
                                                DataType & rightTy,
                                                llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstLShiftOp(AST::ASTNode & callingNode,
                                                  llvm::Constant & leftVal,
                                                  DataType & rightTy,
                                                  llvm::Constant & rightVal) override;
    
    virtual llvm::Constant * codegenConstLRShiftOp(AST::ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal) override;

    virtual llvm::Constant * codegenConstBNotOp(AST::ASTNode & callingNode,
                                                llvm::Constant & val) override;
    
    virtual llvm::Constant * codegenConstPlusOp(AST::ASTNode & callingNode,
                                                llvm::Constant & val) override;
    
    virtual llvm::Constant * codegenConstMinusOp(AST::ASTNode & callingNode,
                                                 llvm::Constant & val) override;
#endif
};

WC_END_NAMESPACE
