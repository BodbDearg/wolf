#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/ArrayLit.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Primitives/ArrayDataType.hpp"
#include "DataType/Primitives/InvalidDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::ArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // If the array has zero elements then that is a compile error, bail out
    //
    // TODO: Someday perhaps we should support zero sized arrays.
    const auto & subExprs = astNode.getExprs();
    const size_t subExprsSize = subExprs.size();
    
    if (subExprsSize <= 0) {
        mCtx.error(astNode, "An array literal must have at least 1 element!");
        return;
    }
    
    // Evaluate each of the sub expressions:
    std::vector<Value> values;
    values.reserve(subExprsSize);
    
    for (const AST::AssignExpr * subExpr : subExprs) {
        subExpr->accept(*this);
        values.push_back(mCtx.popValue());
    }
    
    // Make sure the array element type is valid.
    // The first element defines the type of the array and all element types must match.
    bool allTypesValid = true;
    const CompiledDataType & firstElemCDT = values[0].mCompiledType;
    const DataType & firstElemDT = firstElemCDT.getDataType();
    const AST::ASTNode & firstElemDeclNode = values[0].mDeclaringNode ?
        *values[0].mDeclaringNode :
        astNode;
    
    if (!firstElemDT.isSized()) {
        allTypesValid = false;
        mCtx.error(firstElemDeclNode,
                   "The element type for the array literal must be a sized type, not '%s'!",
                   firstElemDT.name().c_str());
    }
    
    // Make sure all element types are valid and match the first element type:
    bool allElemsValid = values[0].isValid();
    
    for (size_t i = 1; i < subExprsSize; ++i) {
        const Value & elemValue = values[i];
        
        if (!elemValue.isValid()) {
            allElemsValid = false;
        }
        
        if (firstElemDT.isValid()) {
            const DataType & elemDT = elemValue.mCompiledType.getDataType();
            const AST::ASTNode & elemDeclNode = elemValue.mDeclaringNode ?
                *elemValue.mDeclaringNode :
                astNode;
            
            if (!firstElemDT.equals(elemDT)) {
                mCtx.error(elemDeclNode,
                           "Data type for array element #%zu must match the array element type '%s'! "
                           "The data type for the element was '%s'!",
                           size_t(i + 1),
                           firstElemDT.name().c_str(),
                           elemDT.name().c_str());
                
                allTypesValid = false;
            }
        }
    }
    
    // Get or create the compiled data type for the array:
    if (const DataType * arrayDataType = mCtx.getNodeEvaluatedDataType(astNode)) {
        if (arrayDataType->isValid()) {
            arrayDataType->accept(mCodegenDataType);
        }
    }
    else {
        std::unique_ptr<const DataType> evalDataType;
        
        if (allTypesValid) {
            evalDataType.reset(new ArrayDataType(firstElemDT, subExprsSize));
            evalDataType->accept(mCodegenDataType);
        }
        else {
            evalDataType.reset(new InvalidDataType());
        }
        
        mCtx.setNodeEvaluatedDataType(astNode, evalDataType);
    }
    
    CompiledDataType arrayCDT = mCtx.popCompiledDataType();
    
    // Proceed no further if everything isn't valid
    WC_GUARD(arrayCDT.isValid() && allTypesValid && allElemsValid);
    
    // Allocate stack space for the array:
    llvm::AllocaInst * arrayStorage = mCtx.mIRBuilder.CreateAlloca(arrayCDT.getLLVMType());
    WC_ASSERT(arrayStorage);
    
    // Assign each of the element expressions to the array:
    for (size_t i = 0; i < subExprsSize; ++i) {
        // Figure out the pointer to the array element:
        llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCtx.mLLVMCtx), 0);
        WC_ASSERT(zeroIndex);
        llvm::ConstantInt * arrayIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCtx.mLLVMCtx), i);
        WC_ASSERT(arrayIndex);
        llvm::Value * arrayElemPtr = mCtx.mIRBuilder.CreateGEP(arrayStorage, { zeroIndex, arrayIndex });
        WC_ASSERT(arrayElemPtr);
        
        // Do the actual store to the array element.
        llvm::Value * storeResult = mCtx.mIRBuilder.CreateStore(values[i].mLLVMVal, arrayElemPtr);
        WC_ASSERT(storeResult);
    }
    
    // Load the array type:
    llvm::Value * loadedArray = mCtx.mIRBuilder.CreateLoad(arrayStorage, "ArrayLit:Load");
    WC_ASSERT(loadedArray);
    
    // Now push the array type
    mCtx.pushValue(Value(loadedArray, arrayCDT, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
