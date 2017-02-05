#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/ReturnStmnt.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/FuncDataType.hpp"
#include "DataType/Primitives/VoidDataType.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/**
 * Verify that the data type for the type returned is correct for the current function.
 * Returns true if verification is okay, and false otherwise.
 */
static bool verifyReturnTypeForCurrentFunction(Codegen & cg,
                                               const CompiledDataType & typeReturned)
{
    // This should never be called when there isn't a valid function!
    if (!cg.mCtx.mCurFunc) {
        cg.mCtx.error("Internal error! Can't verify the return statement for the current function because "
                      "there is no current function set!");
        
        return false;
    }
    
    // Get the compiled type for the current function
    const DataType & curFuncAbstractDataType = cg.mCtx.mCurFunc->mCompiledType.getDataType();
    
    if (curFuncAbstractDataType.getTypeId() != DataTypeId::kFunc) {
        cg.mCtx.error("Can't verify the return type for a return statement because the data type "
                      "determined for the current function is not valid!");
        
        return false;
    }
    
    // Alright, cast to the function data type and compile the return type
    const FuncDataType & funcDataType = static_cast<const FuncDataType&>(curFuncAbstractDataType);
    funcDataType.mReturnType.accept(cg.mCodegenDataType);
    CompiledDataType funcCompiledRetType = cg.mCtx.popCompiledDataType();
    
    // Now check that the types match:
    if (!funcCompiledRetType.getDataType().equals(typeReturned.getDataType())) {
        cg.mCtx.error("Invalid return type for current function! Function expects return type '%s' "
                      "but instead return statement returns type '%s'!",
                      funcCompiledRetType.getDataType().name().c_str(),
                      typeReturned.getDataType().name().c_str());
    }
    
    // All good
    return true;
}

void Codegen::visit(const AST::ReturnStmntNoCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the void return type
    PrimitiveDataTypes::getVoidDataType().accept(mCodegenDataType);
    CompiledDataType returnedType = mCtx.popCompiledDataType();
    
    // Verify the return type is correct, if it isn't then bail
    WC_GUARD(verifyReturnTypeForCurrentFunction(*this, returnedType));
    
    // Generate the code for the return and return true for success
    mCtx.mIRBuilder.CreateRetVoid();
    
    // Grab the parent function
    auto & irb = mCtx.mIRBuilder;
    llvm::Function * parentFn = irb.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a new basic block for the unreachable code past this and set all future
    // code to insert after it. Also create an 'unreachable' instruction to let llvm know
    // everything past this can't be reached...
    std::string bbLabel = StringUtils::appendLineInfo("ReturnStmntNoCondVoid:unreachable",
                                                      astNode.getPastEndToken());
    
    llvm::BasicBlock * unreachableBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                                bbLabel,
                                                                parentFn);
    
    WC_ASSERT(unreachableBB);
    mCtx.mIRBuilder.SetInsertPoint(unreachableBB);
    mCtx.mIRBuilder.CreateUnreachable();
}

void Codegen::visit(const AST::ReturnStmntNoCondWithValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the assign expression for the return
    astNode.mReturnExpr.accept(*this);
    Value returnVal = mCtx.popValue();
    
    // Verify that the return type is correct:
    WC_GUARD(verifyReturnTypeForCurrentFunction(*this, returnVal.mCompiledType));
    
    // Make sure we have a valid value to return:
    WC_GUARD(returnVal.isValid());
    WC_ASSERT(!returnVal.mRequiresLoad);
    
    // Now generate the return and return true for success
    mCtx.mIRBuilder.CreateRet(returnVal.mLLVMVal);
    
    // Grab the parent function
    auto & irb = mCtx.mIRBuilder;
    llvm::Function * parentFn = irb.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a new basic block for the unreachable code past this and set all future
    // code to insert after it. Also create an 'unreachable' instruction to let llvm know
    // everything past this can't be reached...
    std::string bbLabel = StringUtils::appendLineInfo("ReturnStmntNoCondWithValue:unreachable",
                                                      astNode.getPastEndToken());
    
    llvm::BasicBlock * unreachableBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx,
                                                                bbLabel,
                                                                parentFn);
    
    WC_ASSERT(unreachableBB);
    mCtx.mIRBuilder.SetInsertPoint(unreachableBB);
    mCtx.mIRBuilder.CreateUnreachable();
}

void Codegen::visit(const AST::ReturnStmntWithCondAndValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::ReturnStmntWithCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
