#include "CodegenDataType.hpp"

#include "../CodegenCtx.hpp"
#include "../CompiledDataType.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "DataType/Primitives/ArrayBadSizeDataType.hpp"
#include "DataType/Primitives/ArrayDataType.hpp"
#include "DataType/Primitives/ArrayUnevalSizeDataType.hpp"
#include "DataType/Primitives/BoolDataType.hpp"
#include "DataType/Primitives/FuncDataType.hpp"
#include "DataType/Primitives/FuncUnevalDataType.hpp"
#include "DataType/Primitives/Int64DataType.hpp"
#include "DataType/Primitives/StrDataType.hpp"
#include "DataType/Primitives/VoidDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenDataType::CodegenDataType(CodegenCtx & ctx, ConstCodegen & constCodegen) :
    mCtx(ctx),
    mConstCodegen(constCodegen)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenDataType::visit(const ArrayBadSizeDataType & dataType) {
    // We can't codegen an unknown data type
    WC_UNUSED_PARAM(dataType);
    mCtx.error("Can't codegen the data type for an array with element type '%s' "
               "which has a bad size specification!",
               dataType.mElemType.name().c_str());
}

void CodegenDataType::visit(const ArrayDataType & dataType) {
    // Codegen the element type. If that fails then bail out.
    dataType.mElemType.accept(*this);
    CompiledDataType elemCompiledType = mCtx.popCompiledDataType();
    WC_GUARD(elemCompiledType.getLLVMType());
    
    // The type must be sized in order to be code generated as an array type.
    const DataType & elemDataType = elemCompiledType.getDataType();
    
    if (!elemDataType.isSized()) {
        mCtx.error("Can't generate an array of unsized type '%s'!", elemDataType.name().c_str());
        return;
    }
    
    // The array size must be > 0 for it to be valid
    if (dataType.mSize <= 0) {
        mCtx.error("Can't generate a zero sized array!");
        return;
    }
    
    // Alright, make the array type:
    llvm::Type * llvmType = llvm::ArrayType::get(elemCompiledType.getLLVMType(), dataType.mSize);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const ArrayUnevalSizeDataType & dataType) {
    // If the result of this has been cached then use that instead
    if (const DataType * evaluatedDataType = mCtx.getNodeEvaluatedDataType(dataType.mDeclaringNode)) {
        evaluatedDataType->accept(*this);
        return;
    }
    
    // Codegen the element type:
    dataType.mElemType.accept(*this);
    CompiledDataType elemCompiledType = mCtx.popCompiledDataType();

    // Evaluate the size expression for the array
    dataType.mSizeExpr.accept(mConstCodegen);
    Constant sizeConst = mCtx.popConstant();
    
    // If the size expression is not of type 'int' then it is invalid:
    const DataType & sizeExprDataType = sizeConst.mCompiledType.getDataType();
    bool sizeExprIsInt = sizeExprDataType.isInteger();
    
    if (!sizeExprIsInt) {
        mCtx.error(dataType.mSizeExpr,
                   "Size expression for array size must be an integer type, not '%s'!",
                   sizeExprDataType.name().c_str());
    }
    
    // Save the evaluated data type here:
    std::unique_ptr<const DataType> evaluatedDataType;
    
    if (sizeExprIsInt && sizeConst.isValid()) {
        llvm::ConstantInt * sizeLLVMConst = llvm::dyn_cast<llvm::ConstantInt>(sizeConst.mLLVMConst);
        
        if (!sizeLLVMConst->isNegative()) {
            const auto & sizeAPInt = sizeLLVMConst->getValue();
            
            if (sizeAPInt.getNumWords() <= 1) {
                // Got the array size, set the evaluated array type:
                size_t arraySize = sizeAPInt.getZExtValue();
                evaluatedDataType.reset(new ArrayDataType(elemCompiledType.getDataType(), arraySize));
            }
            else {
                mCtx.error("Array size is too big for array! Max supported size: %zu", UINT64_MAX);
            }
        }
        else {
            mCtx.error(dataType.mSizeExpr, "Size expression for array cannot be negative!");
        }
    }
    
    if (!evaluatedDataType.get()) {
        // Data type could not be evaluated for one reason or another or is not valid.
        // Evaluated data type is hence an array with a bad size...
        evaluatedDataType.reset(new ArrayBadSizeDataType(dataType.mDeclaringNode,
                                                         elemCompiledType.getDataType(),
                                                         dataType.mSizeExpr));
    }
    
    // Codegen the evaluated type:
    evaluatedDataType->accept(*this);
    
    // Save the evaluated type so we don't have to go through this again:
    mCtx.setNodeEvaluatedDataType(dataType.mDeclaringNode, evaluatedDataType);
}

void CodegenDataType::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt1Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const FuncDataType & dataType) {
    // Codegen the return type
    dataType.mReturnType.accept(*this);
    CompiledDataType returnCompiledType = mCtx.popCompiledDataType();
    
    // Codegen all the arg types
    std::vector<CompiledDataType> argCompiledTypes;
    argCompiledTypes.reserve(dataType.mArgTypes.size());
    
    for (const DataType * argDataType : dataType.mArgTypes) {
        argDataType->accept(*this);
        argCompiledTypes.push_back(mCtx.popCompiledDataType());
    }
    
    // See if everything is valid
    bool funcTypeValid = returnCompiledType.isValid();
    
    for (const CompiledDataType & argCompiledType : argCompiledTypes) {
        if (argCompiledType.isValid()) {
            const DataType & argDataType = argCompiledType.getDataType();
            
            if (!argDataType.isSized()) {
                mCtx.error("An argument of unsized type '%s' is not allowed for a function!",
                           argDataType.name().c_str());
            }
        }
        else {
            funcTypeValid = false;
        }
    }
    
    // Create a list of the llvm argument types:
    std::vector<llvm::Type*> argLLVMTypes;
    argLLVMTypes.reserve(argCompiledTypes.size());
    
    for (const CompiledDataType & argCompiledType : argCompiledTypes) {
        argLLVMTypes.push_back(argCompiledType.getLLVMType());
    }
    
    // If everything is valid then create the llvm type
    llvm::Type * funcLLVMType = nullptr;
    
    if (funcTypeValid) {
        // Create the function signature:
        // TODO: support varargs
        // TODO: support different linkage types
        funcLLVMType = llvm::FunctionType::get(returnCompiledType.getLLVMType(),
                                               argLLVMTypes,
                                               false);
        
        WC_ASSERT(funcLLVMType);
    }
    
    // Now save the result:
    mCtx.pushCompiledDataType(CompiledDataType(dataType, funcLLVMType));
}

void CodegenDataType::visit(const FuncUnevalDataType & dataType) {
    // If the result of this has been cached then use that instead
    if (const DataType * evaluatedDataType = mCtx.getNodeEvaluatedDataType(dataType.mDeclaringNode)) {
        evaluatedDataType->accept(*this);
        return;
    }
    
    // Codegen the return type
    dataType.mReturnType.accept(*this);
    CompiledDataType returnCompiledType = mCtx.popCompiledDataType();
    
    // Codegen all the arg types
    std::vector<CompiledDataType> argCompiledTypes;
    argCompiledTypes.reserve(dataType.mArgTypes.size());
    
    for (const DataType * argDataType : dataType.mArgTypes) {
        argDataType->accept(*this);
        argCompiledTypes.push_back(mCtx.popCompiledDataType());
    }
    
    // Make a list of arg data types
    std::vector<const DataType*> argDataTypes;
    argDataTypes.reserve(argCompiledTypes.size());
    
    for (CompiledDataType & compiledType : argCompiledTypes) {
        argDataTypes.push_back(&compiledType.getDataType());
    }
    
    // Save the evaluated data type here:
    std::unique_ptr<const DataType> evaluatedDataType;
    evaluatedDataType.reset(new FuncDataType(returnCompiledType.getDataType(),
                                             std::move(argDataTypes)));
    
    // Codegen the evaluated type:
    evaluatedDataType->accept(*this);
    
    // Save the evaluated type so we don't have to go through this again:
    mCtx.setNodeEvaluatedDataType(dataType.mDeclaringNode, evaluatedDataType);
}

void CodegenDataType::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt64Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const StrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt8PtrTy(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const InvalidDataType & dataType) {
    // We can't codegen an unknown data type
    WC_UNUSED_PARAM(dataType);
    mCtx.error("Can't codegen an invalid data type!");
}

void CodegenDataType::visit(const UnevalDataType & dataType) {
    // We can't codegen an unevaluated data type
    WC_UNUSED_PARAM(dataType);
    mCtx.error("Can't codegen an unevaluated data type!");
}

void CodegenDataType::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getVoidTy(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
