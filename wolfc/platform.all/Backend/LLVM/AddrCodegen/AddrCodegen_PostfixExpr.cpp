//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AddrCodegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/ArrayDataType.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void AddrCodegen::visit(const AST::PostfixExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PostfixExprInc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PostfixExprDec & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void AddrCodegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Evaluate the expression first:
    astNode.accept(mCodegen);
    Value exprVal = mCtx.popValue();
    
    // The value must be valid for this to work
    WC_GUARD(exprVal.isValid());
    
    // The data type for the return value must be sized
    const CompiledDataType & exprValCDT = exprVal.mCompiledType;
    const DataType & exprValDT = exprValCDT.getDataType();
    
    if (!exprValDT.isSized()) {
        // Note: no error in the case of an 'undefined' type since this means an error was already emitted elsewhere.
        if (!exprValDT.isUndefined()) {
            mCtx.error(astNode,
                       "Can't take the address of a function call result which returns "
                       "unsized type '%s'!",
                       exprValDT.name().c_str());
        }
        
        return;
    }
    
    // Create an alloca to hold the result of the function call and store the result there.
    llvm::Value * llvmStackVal = mCtx.mIRBuilder.CreateAlloca(exprValCDT.getLLVMType(),
                                                              nullptr,
                                                              "AddrCodegen:PostfixExprFuncCall:Alloca");
    
    WC_ASSERT(llvmStackVal);
    mCtx.mIRBuilder.CreateStore(exprVal.mLLVMVal, llvmStackVal);
    
    // Push it onto the codegen context stack
    mCtx.pushValue(Value(llvmStackVal, exprValCDT, true, &astNode));
}

void AddrCodegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codgen the address of the 'array' expression first.
    // Note that the array expresion might not neccesarily be an array, could potentially be any other
    // type that we support indexing on (such as pointers).
    astNode.mArrayExpr.accept(*this);
    Value arrayExprAddr = mCtx.popValue();
    WC_ASSERT(!arrayExprAddr.isValid() || arrayExprAddr.mRequiresLoad);
    
    // Codegen the expression for the array index
    astNode.mIndexExpr.accept(mCodegen);
    Value indexVal = mCtx.popValue();
    WC_ASSERT(!indexVal.isValid() || !indexVal.mRequiresLoad);
    
    // Figure out if we can do array indexing on the type being indexed and get the element type.
    //
    // TODO: support the array lookup operator on custom types eventually.
    const CompiledDataType & arrayExprCDT = arrayExprAddr.mCompiledType;
    const DataType & arrayExprDT = arrayExprCDT.getDataType();
    const DataType * arrayElemDT = nullptr;
    bool exprTypeAndIndexTypesAreOk = true;
    
    if (arrayExprDT.isArray()) {
        // Indexing an array
        const ArrayDataType & arrayDT = static_cast<const ArrayDataType&>(arrayExprDT);
        arrayElemDT = &arrayDT.mElemType;
    }
    else if (arrayExprDT.isPtr()) {
        // Indexing a pointer, similar to how it is done in the 'C' language
        const PtrDataType & ptrDT = static_cast<const PtrDataType&>(arrayExprDT);
        arrayElemDT = &ptrDT.mPointedToType;
        
        // The pointer value must be loaded first though:
        arrayExprAddr.mLLVMVal = mCtx.mIRBuilder.CreateLoad(arrayExprAddr.mLLVMVal, "AddrCodegen:PostfixExprArrayLookup:LoadPtrVal");
        arrayExprAddr.mRequiresLoad = false;
        WC_ASSERT(arrayExprAddr.mLLVMVal);
    }
    else {
        // Note: no error in the case of an 'undefined' type since this means an error was already emitted elsewhere.
        if (!arrayExprDT.isUndefined()) {
            // TODO: this message will need to be updated once we support indexing on user types
            mCtx.error("Can't perform array indexing on an expression of type '%s'! "
                       "Currently only arrays and pointer data types can be indexed.",
                       arrayExprDT.name().c_str());
        }
        
        exprTypeAndIndexTypesAreOk = false;
    }
    
    // Index expression must be an integer
    const CompiledDataType & indexCDT = indexVal.mCompiledType;
    const DataType & indexDT = indexCDT.getDataType();
    
    if (!indexDT.isInteger()) {
        // Note: no error in the case of an 'undefined' type since this means an error was already emitted elsewhere.
        if (!indexDT.isUndefined()) {
            mCtx.error("Index expression for array lookup must be an integer not type '%s'! "
                       "Can't index an array with non-integer types!",
                       indexDT.name().c_str());
        }

        exprTypeAndIndexTypesAreOk = false;
    }
    
    // Proceed no further if any of these are invalid
    WC_GUARD(indexVal.isValid() &&
             arrayExprAddr.isValid() &&
             exprTypeAndIndexTypesAreOk);
    
    // Get the value for the array address:
    llvm::Value * arrayElemAddr = nullptr;
    
    if (arrayExprDT.isArray()) {
        // Array indexing an array
        llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCtx.mLLVMCtx), 0);
        WC_ASSERT(zeroIndex);
        arrayElemAddr = mCtx.mIRBuilder.CreateGEP(arrayExprAddr.mLLVMVal,
                                                  { zeroIndex, indexVal.mLLVMVal },
                                                  "AddrCodegen:PostfixExprArrayLookup:ElemAddr");
    }
    else {
        // Array indexing a pointer
        arrayElemAddr = mCtx.mIRBuilder.CreateGEP(arrayExprAddr.mLLVMVal,
                                                  indexVal.mLLVMVal,
                                                  "AddrCodegen:PostfixExprArrayLookup:ElemAddr");
    }
    
    WC_ASSERT(arrayElemAddr);
    
    // Figure out the compiled data type for the element type:
    WC_ASSERT(arrayElemDT);
    arrayElemDT->accept(mCodegenDataType);
    CompiledDataType arrayElemCDT = mCtx.popCompiledDataType();
    WC_GUARD(arrayElemCDT.isValid());
    
    // All good, save the result!
    mCtx.pushValue(Value(arrayElemAddr, arrayElemCDT, true, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
