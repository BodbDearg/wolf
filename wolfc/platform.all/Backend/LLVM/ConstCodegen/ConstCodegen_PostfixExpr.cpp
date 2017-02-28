#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/ArrayDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::PostfixExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::PostfixExprInc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprInc");
}

void ConstCodegen::visit(const AST::PostfixExprDec & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    codegenNotSupportedForNodeTypeError(astNode, "PostfixExprDec");
}

void ConstCodegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    mCtx.error(astNode, "Function calls cannot be evaluated at compile time! (yet)");
}

void ConstCodegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Codegen the array constant
    astNode.mArrayExpr.accept(*this);
    Constant arrayConst = mCtx.popConstant();
    
    // Codegen the index expression
    astNode.mIndexExpr.accept(*this);
    Constant indexConst = mCtx.popConstant();
    
    // Make sure the array is actually an array...
    // TODO: support this operator on custom types eventually
    bool arrayIsValid = true;
    
    if (arrayConst.isValid()) {
        const DataType & arrayDT = arrayConst.mCompiledType.getDataType();
        
        if (!arrayDT.isArray()) {
            mCtx.error(astNode, "Can't perform array indexing on an expression that is not an array!");
            arrayIsValid = false;
        }
    }
    else {
        arrayIsValid = false;
    }
    
    // Make sure the index is valid
    WC_GUARD(indexConst.isValid());
    const DataType & indexDT = indexConst.mCompiledType.getDataType();
    
    // The index must be an integer
    if (!indexDT.isInteger()) {
        mCtx.error("Index expression for array lookup must be an integer not type '%s'! "
                   "Can't index an array with non-integer types!",
                   indexDT.name().c_str());
        
        return;
    }
    
    // Make sure the index expression fits in 64-bits
    llvm::APInt indexAPInt = indexConst.mLLVMConst->getUniqueInteger();
    
    if (indexAPInt.getActiveBits() > 64) {
        mCtx.error("Index expression for array lookup is too big! Does not fit in 64-bits! "
                   "Number requires '%zu' bits to store!",
                   uint64_t(indexAPInt.getActiveBits()));
        
        return;
    }
    
    // The index expression cannot be negative
    if (indexAPInt.isNegative()) {
        mCtx.error("Index expression for array lookup cannot be negative! "
                   "Value given was: %zi",
                   int64_t(indexAPInt.getSExtValue()));
        
        return;
    }
    
    // Get the actual index value
    uint64_t index = indexAPInt.getZExtValue();
    
    // Okay, make sure the index is in bounds
    const ArrayDataType & arrayDT = static_cast<const ArrayDataType&>(arrayConst.mCompiledType.getDataType());
    
    if (index >= arrayDT.mSize) {
        mCtx.error(astNode.mIndexExpr,
                   "Index '%zu' is out of range for the array! Array size is: %zu",
                   index,
                   uint64_t(arrayDT.mSize));
        
        return;
    }
    
    // Due to LLVM API restrictions we can only support as big as UINT_MAX here for an index because
    // getExtractElement() takes this data type rather than uint64_t...
    //
    // It may be possible to lift this restriction in future, though I doubt this limitation will be
    // much of an issue in practice. Who throws arrays >~ 4 billion elements at their compilers anyways?!
    if (index > UINT_MAX) {
        mCtx.error("Array index '%zu' is too large! "
                   "Can only support a max array index of '%zu' due to LLVM API restrictions!",
                   index,
                   uint64_t(UINT_MAX));
        
        return;
    }
    
    // Can't do anything else if the array is not valid
    WC_GUARD(arrayIsValid);
    
    // Get the datatype for the result (element type)
    arrayDT.mElemType.accept(mCodegenDataType);
    CompiledDataType arrayElemCDT = mCtx.popCompiledDataType();
    
    // Get the element constant value
    llvm::Constant * elemConst = llvm::ConstantExpr::getExtractValue(arrayConst.mLLVMConst, { unsigned(index) });
    WC_ASSERT(elemConst);
    
    // Okay, save the result to the codegen context:
    mCtx.pushConstant(Constant(elemConst, arrayElemCDT, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
