#pragma once

#include "CompiledDataType.hpp"

namespace llvm {
    class Constant;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* Struct holding an LLVM constant and it's corresponding compiled data type */
struct Constant {
    Constant() :
        mLLVMConst(nullptr),
        mCompiledType(),
        mDeclaringNode(nullptr)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Constant(llvm::Constant * llvmConst,
             const CompiledDataType & compiledType,
             const AST::ASTNode * declaringNode)
    :
        mLLVMConst(llvmConst),
        mCompiledType(compiledType),
        mDeclaringNode(declaringNode)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Constant(const Constant & other) = default;
    Constant & operator = (const Constant & other) = default;
    
    inline bool isValid() const {
        return mLLVMConst != nullptr && mCompiledType.isValid();
    }
    
    /* The llvm constant representing the constant */
    llvm::Constant * mLLVMConst;
    
    /* The compiled data type for this constant */
    CompiledDataType mCompiledType;
    
    /* The node that declared the value */
    const AST::ASTNode * mDeclaringNode;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
