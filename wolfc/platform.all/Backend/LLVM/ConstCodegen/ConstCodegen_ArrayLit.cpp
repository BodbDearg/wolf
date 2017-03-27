//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/ArrayLit.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "Assert.hpp"
#include "DataType/Types/ArrayDataType.hpp"
#include "DataType/Types/UndefinedDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::ArrayLit & astNode) {
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
    std::vector<Constant> constants;
    constants.reserve(subExprsSize);
    
    for (const AST::AssignExpr * subExpr : subExprs) {
        subExpr->accept(*this);
        constants.push_back(mCtx.popConstant());
    }
    
    // Make sure the array element type is valid.
    // The first element defines the type of the array and all element types must match.
    bool allTypesValid = true;
    const CompiledDataType & firstElemCDT = constants[0].mCompiledType;
    const DataType & firstElemDT = firstElemCDT.getDataType();
    const AST::ASTNode & firstElemDeclNode = constants[0].mDeclaringNode ?
        *constants[0].mDeclaringNode :
        astNode;
    
    if (!firstElemDT.isSized()) {
        allTypesValid = false;
        mCtx.error(firstElemDeclNode,
                   "The element type for the array literal must be a sized type, not '%s'!",
                   firstElemDT.name().c_str());
    }
    
    // Make sure all element types are valid and match the first element type:
    bool allElemsValid = constants[0].isValid();
    
    for (size_t i = 1; i < subExprsSize; ++i) {
        const Constant & elemConstant = constants[i];
        
        if (!elemConstant.isValid()) {
            allElemsValid = false;
        }
        
        if (firstElemDT.isValid()) {
            const DataType & elemDT = elemConstant.mCompiledType.getDataType();
            const AST::ASTNode & elemDeclNode = elemConstant.mDeclaringNode ?
                *elemConstant.mDeclaringNode :
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
    
    // Now create the constant array and push to the codegen context
    if (arrayCDT.isValid() && allTypesValid && allElemsValid) {
        // Populate the array of constants
        std::vector<llvm::Constant*> arraySubConstants;
        arraySubConstants.reserve(subExprsSize);
        
        for (const Constant & constant : constants) {
            WC_ASSERT(constant.mLLVMConst);
            arraySubConstants.push_back(constant.mLLVMConst);
        }
        
        // Get the llvm array data type
        llvm::ArrayType * llvmArrayType = llvm::cast<llvm::ArrayType>(arrayCDT.getLLVMType());
        WC_ASSERT(llvmArrayType);
        
        // Create the array:
        llvm::Constant * llvmArray = llvm::ConstantArray::get(llvmArrayType, arraySubConstants);
        WC_ASSERT(llvmArray);
        mCtx.pushConstant(Constant(llvmArray, arrayCDT, &astNode));
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
