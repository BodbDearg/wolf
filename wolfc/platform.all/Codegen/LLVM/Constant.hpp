#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Constant;
}

WC_BEGIN_NAMESPACE

class DataType;

namespace AST {
    class ASTNode;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

/* Struct holding an LLVM constant and it's corresponding data type */
struct Constant {
    Constant() :
        mDeclaringNode(nullptr),
        mLLVMConstant(nullptr),
        mType(nullptr)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Constant(AST::ASTNode * declaringNode,
             llvm::Constant * llvmConstant,
             const DataType * type)
    :
        mDeclaringNode(declaringNode),
        mLLVMConstant(llvmConstant),
        mType(type)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Constant(const Constant & other) = default;
    Constant & operator = (const Constant & other) = default;
    
    /* The node that declared the value */
    const AST::ASTNode * mDeclaringNode;
    
    /* The llvm constant representing the constant */
    llvm::Constant * mLLVMConstant;
    
    /* The data type for this constant */
    const DataType * mType;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
