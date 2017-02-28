#include "CodegenDataType.hpp"

#include "../CodegenCtx.hpp"
#include "../CompiledDataType.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/Func.hpp"
#include "AST/Nodes/FuncArg.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "AST/Nodes/Type.hpp"
#include "Assert.hpp"
#include "DataType/Types/ArrayDataType.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/FuncDataType.hpp"
#include "DataType/Types/IntDataTypes.hpp"
#include "DataType/Types/InvalidDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "DataType/Types/PtrDataType.hpp"
#include "DataType/Types/StrDataType.hpp"
#include "DataType/Types/VoidDataType.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* A useful macro for data type codegen */
#define WC_RETURN_CACHED_DATA_TYPE_FOR_NODE_IF_AVAILABLE(astNode)\
    /* If the result of this has been cached then use that instead */\
    if (const DataType * evaluatedDataType = mCtx.getNodeEvaluatedDataType(astNode)) {\
        /* Only codegen if valid, don't keep spewing out errors for the same thing over and over. */\
        /* If we have an invalid type generated for a node then we've already done errors for it earlier. */\
        if (evaluatedDataType->isValid()) {\
            evaluatedDataType->accept(*this);\
        }\
        \
        return;\
    }

CodegenDataType::CodegenDataType(CodegenCtx & ctx, ConstCodegen & constCodegen) :
    mCtx(ctx),
    mConstCodegen(constCodegen)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenDataType::visitASTNode(const AST::Func & func) {
    WC_RETURN_CACHED_DATA_TYPE_FOR_NODE_IF_AVAILABLE(func);
    
    // Generate the compiled data type for the function return type.
    // If no explicit data type for the function return is given, assume 'void':
    if (const AST::Type * explicitReturnType = func.getExplicitReturnType()) {
        explicitReturnType->accept(mConstCodegen);
    }
    else {
        // No explicit return type for function, void assumed:
        PrimitiveDataTypes::getVoidDataType().accept(*this);
    }
    
    CompiledDataType returnCDT = mCtx.popCompiledDataType();
    
    // Alright, figure out the compiled data types for all the params:
    const auto & funcArgs = func.getArgs();
    std::vector<CompiledDataType> funcArgCDTs;
    funcArgCDTs.reserve(funcArgs.size());
    
    for (const AST::FuncArg * funcArg : funcArgs) {
        visitASTNode(*funcArg);
        funcArgCDTs.push_back(mCtx.popCompiledDataType());
    }
    
    // Make a list of data type pointers for the 'FuncDataType' constructor
    std::vector<const DataType*> funcArgDTs;
    funcArgDTs.reserve(funcArgCDTs.size());
    
    for (const CompiledDataType & argCDT : funcArgCDTs) {
        funcArgDTs.push_back(&argCDT.getDataType());
    }
    
    // Now, given that we have all the info we need make the the data type and save:
    const FuncDataType * funcDataType = new FuncDataType(returnCDT.getDataType(), std::move(funcArgDTs));
    
    {
        std::unique_ptr<const DataType> ownershipPtr(funcDataType);
        mCtx.setNodeEvaluatedDataType(func, ownershipPtr);
    }
    
    // Codegen the func data type
    funcDataType->accept(*this);
}

void CodegenDataType::visitASTNode(const AST::FuncArg & funcArg) {
    // 'void' cannot be accepted as a valid function arg.
    // Doing checks for that here:
    funcArg.mType.accept(mConstCodegen);
    CompiledDataType funcArgCDT = mCtx.popCompiledDataType();
    const DataType & funcArgDT = funcArgCDT.getDataType();
    
    if (funcArgDT.isSized()) {
        // All is good, push the arg back onto the stack
        mCtx.pushCompiledDataType(funcArgCDT);
    }
    else {
        // Only generate the 'void' arg error if the data type is actually valid
        if (funcArgDT.isValid()) {
            // Invalid unsized type like 'void' passed as func arg type
            mCtx.error(funcArg,
                       "Invalid unsized data type '%s' for function argument!",
                       funcArgDT.name().c_str());
        }
    }
}

void CodegenDataType::visit(const ArrayDataType & dataType) {
    // Codegen the element type. If that fails then bail out.
    dataType.mElemType.accept(*this);
    CompiledDataType elemCDT = mCtx.popCompiledDataType();
    WC_GUARD(elemCDT.getLLVMType());
    
    // The type must be sized in order to be code generated as an array type.
    const DataType & elemDataType = elemCDT.getDataType();
    
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
    llvm::Type * llvmType = llvm::ArrayType::get(elemCDT.getLLVMType(), dataType.mSize);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visitASTNode(const AST::PrimitiveType & primitiveType) {
    // Get the data type object for this primitive and codegen
    const DataType & dataType = PrimitiveDataTypes::getUsingLangKeyword(primitiveType.mToken.type);
    dataType.accept(*this);
}

void CodegenDataType::visitASTNode(const AST::TypeArray & typeArray) {
    WC_RETURN_CACHED_DATA_TYPE_FOR_NODE_IF_AVAILABLE(typeArray);
    
    // Codegen the element type:
    typeArray.mElemType.accept(mConstCodegen);
    CompiledDataType elemCDT = mCtx.popCompiledDataType();
    
    // Evaluate the size expression for the array
    typeArray.mSizeExpr.accept(mConstCodegen);
    Constant sizeConst = mCtx.popConstant();
    
    // If the size expression is not of type 'int' then it is invalid:
    const DataType & sizeExprDataType = sizeConst.mCompiledType.getDataType();
    bool sizeExprIsInt = sizeExprDataType.isInteger();
    
    if (!sizeExprIsInt) {
        mCtx.error(typeArray.mSizeExpr,
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
                //
                // TODO: Use linear allocator here?
                size_t arraySize = sizeAPInt.getZExtValue();
                evaluatedDataType.reset(new ArrayDataType(elemCDT.getDataType(), arraySize));
            }
            else {
                mCtx.error("Array size is too big for array! Max supported size: %zu", UINT64_MAX);
            }
        }
        else {
            mCtx.error(typeArray.mSizeExpr, "Size expression for array cannot be negative!");
        }
    }
    
    // If an error occurred this evaluates to the invalid data type
    if (!evaluatedDataType.get()) {
        // Data type could not be evaluated for one reason or another or is not valid.
        // Evaluated data type is hence an array with a bad size...
        //
        // TODO: Use linear allocator here?
        evaluatedDataType.reset(new InvalidDataType());
    }
    
    // Codegen the evaluated type (if valid).
    // If it's not valid we don't do anything because we've already done errors earlier.
    if (evaluatedDataType->isValid()) {
        evaluatedDataType->accept(*this);
    }
    
    // Save the evaluated type so we don't have to go through this again:
    mCtx.setNodeEvaluatedDataType(typeArray, evaluatedDataType);
}

void CodegenDataType::visitASTNode(const AST::TypePtr & typePtr) {
    WC_RETURN_CACHED_DATA_TYPE_FOR_NODE_IF_AVAILABLE(typePtr);
    
    // Codgen the type pointed to
    typePtr.mPointedToType.accept(mConstCodegen);
    CompiledDataType pointedToTypeCDT = mCtx.popCompiledDataType();
    
    // Save the evaluated data type here:
    //
    // TODO: Use linear allocator here?
    std::unique_ptr<const DataType> evaluatedDataType;
    PtrDataType * ptrDataType = new PtrDataType(pointedToTypeCDT.getDataType(), typePtr.isNullablePtr());
    evaluatedDataType.reset(ptrDataType);
    
    // Codegen the evaluated type (if valid).
    // If it's not valid we don't do anything because we've already done errors earlier.
    if (evaluatedDataType->isValid()) {
        evaluatedDataType->accept(*this);
    }
    
    // Save the evaluated type so we don't have to go through this again:
    mCtx.setNodeEvaluatedDataType(typePtr, evaluatedDataType);
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
    CompiledDataType returnTypeCDT = mCtx.popCompiledDataType();
    
    // Codegen all the arg types
    std::vector<CompiledDataType> argCompiledTypes;
    argCompiledTypes.reserve(dataType.mArgTypes.size());
    
    for (const DataType * argDataType : dataType.mArgTypes) {
        argDataType->accept(*this);
        argCompiledTypes.push_back(mCtx.popCompiledDataType());
    }
    
    // See if everything is valid
    bool funcTypeValid = returnTypeCDT.isValid();
    
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
        //
        // TODO: support varargs
        // TODO: support different linkage types
        funcLLVMType = llvm::FunctionType::get(returnTypeCDT.getLLVMType(),
                                               argLLVMTypes,
                                               false);
        
        WC_ASSERT(funcLLVMType);
    }
    
    // Now save the result:
    mCtx.pushCompiledDataType(CompiledDataType(dataType, funcLLVMType));
}

void CodegenDataType::visit(const Int128DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt128Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const Int16DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt16Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const Int32DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt32Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt64Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const Int8DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt8Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const UInt128DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt128Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const UInt16DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt16Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const UInt32DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt32Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const UInt64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt64Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const UInt8DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getInt8Ty(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

void CodegenDataType::visit(const PtrDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    
    // Get the data type for the pointed to type. If it's invalid then proceed no further:
    dataType.mPointedToType.accept(*this);
    CompiledDataType pointedToTypeCDT = mCtx.popCompiledDataType();
    WC_GUARD(pointedToTypeCDT.isValid());
    
    // Now make a pointer to that type and push the result:
    llvm::Type * llvmType = pointedToTypeCDT.getLLVMType()->getPointerTo();
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

void CodegenDataType::visit(const VoidDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    llvm::Type * llvmType = llvm::Type::getVoidTy(mCtx.mLLVMCtx);
    WC_ASSERT(llvmType);
    mCtx.pushCompiledDataType(CompiledDataType(dataType, llvmType));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
