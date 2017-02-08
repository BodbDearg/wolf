#include "AddrCodegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Primitives/ArrayDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void AddrCodegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codgen the address of the array first
    astNode.mArrayExpr.accept(*this);
    Value arrayAddrVal = mCtx.popValue();
    WC_ASSERT(!arrayAddrVal.isValid() || arrayAddrVal.mRequiresLoad);
    
    // Codegen the expression for the array index
    astNode.mIndexExpr.accept(mCodegen);
    Value indexVal = mCtx.popValue();
    WC_ASSERT(!indexVal.isValid() || !indexVal.mRequiresLoad);
    
    // Make sure the array is actually an array and figure out the element type.
    //
    // TODO: support the array lookup operator on custom types eventually.
    const CompiledDataType & arrayCDT = arrayAddrVal.mCompiledType;
    const DataType & arrayAbstractDT = arrayCDT.getDataType();
    
    if (!arrayAbstractDT.isArray()) {
        mCtx.error("Can't perform array indexing on an expression of type '%s'! Only arrays can be indexed.",
                   arrayAbstractDT.name().c_str());
        
        return;
    }
    
    const ArrayDataType & arrayDT = static_cast<const ArrayDataType&>(arrayAbstractDT);

    // Index expression must be an integer
    const CompiledDataType & indexCDT = indexVal.mCompiledType;
    const DataType & indexDT = indexCDT.getDataType();
    
    if (!indexDT.isInteger()) {
        mCtx.error("Index expression for array lookup must be an integer not type '%s'! "
                   "Can't index an array with non-integer types!",
                   indexDT.name().c_str());

        return;
    }
    
    // Proceed no further if either of these is invalid
    WC_GUARD(indexVal.isValid() && arrayAddrVal.isValid());
    
    // Get the value for the array address:
    llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCtx.mLLVMCtx), 0);
    WC_ASSERT(zeroIndex);
    llvm::Value * arrayElemAddr = mCtx.mIRBuilder.CreateGEP(arrayAddrVal.mLLVMVal, { zeroIndex, indexVal.mLLVMVal });
    WC_ASSERT(arrayElemAddr);
    
    // Figure out the compiled data type for the value loaded. If we fail in this then bail:
    arrayDT.mElemType.accept(mCodegenDataType);
    CompiledDataType arrayElemCDT = mCtx.popCompiledDataType();
    WC_GUARD(arrayElemCDT.isValid());
    
    // All good, save the result!
    mCtx.pushValue(Value(arrayElemAddr, arrayElemCDT, true, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
