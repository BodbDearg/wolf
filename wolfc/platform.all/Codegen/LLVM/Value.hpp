#pragma once

#include "CompiledDataType.hpp"

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

/* Struct holding an LLVM value and it's corresponding compiled data type */
struct Value {
    Value() :
        mLLVMVal(nullptr),
        mCompiledType(),
        mRequiresLoad(false),
        mDeclaringNode(nullptr)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Value(llvm::Value * llvmVal,
          const CompiledDataType & compiledType,
          bool requiresLoad,
          const AST::ASTNode * declaringNode)
    :
        mLLVMVal(llvmVal),
        mCompiledType(compiledType),
        mRequiresLoad(requiresLoad),
        mDeclaringNode(declaringNode)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Value(const Value & other) = default;
    Value & operator = (const Value & other) = default;
    
    inline bool isValid() const {
        return mLLVMVal != nullptr && mCompiledType.isValid();
    }
    
    /* The llvm value representing the value */
    llvm::Value * mLLVMVal;
    
    /* The compiled data type for this value */
    CompiledDataType mCompiledType;
    
    /* If true the value requires a load first before being used */
    bool mRequiresLoad;
    
    /* The node that declared the value */
    const AST::ASTNode * mDeclaringNode;    
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
