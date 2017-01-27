#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class DataType;

namespace AST {
    class ASTNode;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

/* Struct holding an LLVM value and it's corresponding data type */
struct Value {
    Value() :
        mDeclaringNode(nullptr),
        mLLVMValue(nullptr),
        mType(nullptr),
        mRequiresLoad(false)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Value(AST::ASTNode * declaringNode,
          llvm::Value * llvmValue,
          const DataType * type,
          bool requiresLoad)
    :
        mDeclaringNode(declaringNode),
        mLLVMValue(llvmValue),
        mType(type),
        mRequiresLoad(requiresLoad)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Value(const Value & other) = default;
    Value & operator = (const Value & other) = default;
    
    /* The node that declared the value */
    const AST::ASTNode * mDeclaringNode;
    
    /* The llvm value representing the value */
    llvm::Value * mLLVMValue;
    
    /* The data type for this value */
    const DataType * mType;
    
    /* If true the value requires a load first before being used */
    bool mRequiresLoad;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
