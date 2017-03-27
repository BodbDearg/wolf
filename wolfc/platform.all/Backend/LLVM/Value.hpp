//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "CompiledDataType.hpp"
#include "DataType/DataType.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* Struct holding an LLVM value and it's corresponding compiled data type */
struct Value {
    Value() :
        mName(),
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
        mName(),
        mLLVMVal(llvmVal),
        mCompiledType(compiledType),
        mRequiresLoad(requiresLoad),
        mDeclaringNode(declaringNode)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    Value(const std::string & name,
          llvm::Value * llvmVal,
          const CompiledDataType & compiledType,
          bool requiresLoad,
          const AST::ASTNode * declaringNode)
    :
        mName(name),
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
        if (mCompiledType.isValid()) {
            // Note: for 'void' values allow the llvm value to be null.
            // This does not affect the validity for void values.
            if (mCompiledType.getDataType().isVoid()) {
                return true;
            }
            
            return mLLVMVal != nullptr;
        }
        
        return false;
    }
    
    inline bool isVoid() const {
        return mCompiledType.getDataType().isVoid();
    }
    
    /* The name of the value */
    std::string mName;
    
    /* The llvm value representing the value */
    llvm::Value * mLLVMVal;
    
    /* The compiled data type for this value */
    CompiledDataType mCompiledType;
    
    /* If true the value requires a load first before being used */
    bool mRequiresLoad;
    
    /* The node that declared the value */
    const AST::ASTNode * mDeclaringNode;    
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
