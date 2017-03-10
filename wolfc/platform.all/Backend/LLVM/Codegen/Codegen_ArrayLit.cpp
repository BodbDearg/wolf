//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/ArrayLit.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/ArrayDataType.hpp"
#include "DataType/Types/UndefinedDataType.hpp"
#include "NoRTTIOps.hpp"

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
    bool allExprsValid = true;
    std::vector<Value> values;
    values.reserve(subExprsSize);
    
    for (const AST::AssignExpr * subExpr : subExprs) {
        subExpr->accept(*this);
        values.push_back(mCtx.popValue());
        const Value & value = values.back();
        
        if (!value.isValid()) {
            allExprsValid = false;
        }
    }
    
    // Continue no further if everything is invalid:
    WC_GUARD(allExprsValid);
    
    // Make sure the array element type is valid.
    // The first element defines the type of the array and all element types must match.
    // If it is not valid then bail out at this particular point.
    const CompiledDataType & firstElemCDT = values[0].mCompiledType;
    const DataType & firstElemDT = firstElemCDT.getDataType();
    
    if (!firstElemDT.isSized()) {
        const AST::ASTNode & firstElemDeclNode = values[0].mDeclaringNode ?
            *values[0].mDeclaringNode :
            astNode;
        
        mCtx.error(firstElemDeclNode,
                   "The element type for the array literal must be a sized type, not '%s'!",
                   firstElemDT.name().c_str());
        
        return;
    }
    
    // Make sure all element types are valid and match the first element type:
    bool allTypesValid = true;
    
    for (size_t i = 1; i < subExprsSize; ++i) {
        const Value & elemValue = values[i];

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
            evalDataType.reset(new UndefinedDataType());
        }
        
        mCtx.setNodeEvaluatedDataType(astNode, evalDataType);
    }
    
    CompiledDataType arrayCDT = mCtx.popCompiledDataType();
    
    // Proceed no further if everything isn't valid
    WC_GUARD(arrayCDT.isValid() && allTypesValid);
    
    // Create an undefined value for this array:
    llvm::Value * arrayUndefVal = llvm::UndefValue::get(arrayCDT.getLLVMType());
    
    // Assign each of the element expressions to the array:
    for (size_t index = 0; index < subExprsSize; ++index) {
        // Due to LLVM API restrictions we can only support as big as UINT_MAX here for an index
        // because CreateInsertValue() takes this data type rather than uint64_t...
        //
        // It may be possible to lift this restriction in future, though I doubt this limitation will be
        // much of an issue in practice. Who throws arrays >~ 4 billion elements at their
        // compilers anyways?!
        if (index > UINT_MAX) {
            mCtx.error(*subExprs[index],
                       "Array index '%zu' is too large! "
                       "Can only support a max array index of '%zu' due to LLVM API restrictions!",
                       index,
                       uint64_t(UINT_MAX));
            
            return;
        }
        
        // Do the actual store to the array element.
        arrayUndefVal = NoRTTIOps::IRB::CreateInsertValue(mCtx.mIRBuilder,
                                                          *arrayUndefVal,
                                                          *values[index].mLLVMVal,
                                                          { static_cast<unsigned>(index) },
                                                          "ArrayLit:InsertVal");
    }
    
    // Now push the array type
    mCtx.pushValue(Value(arrayUndefVal, arrayCDT, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
