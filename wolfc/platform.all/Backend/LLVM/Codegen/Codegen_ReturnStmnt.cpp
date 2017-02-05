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

void Codegen::visit(const AST::ReturnStmntWithCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the void return type
    PrimitiveDataTypes::getVoidDataType().accept(mCodegenDataType);
    CompiledDataType returnedType = mCtx.popCompiledDataType();

    // Codegen the conditional expression
    astNode.mCondExpr.accept(*this);
    Value condVal = mCtx.popValue();
    
    // Verify that the condition value is a bool
    const DataType & condValType = condVal.mCompiledType.getDataType();
    bool condValIsBool = true;
    
    if (!condValType.isBool()) {
        mCtx.error(astNode.mCondExpr,
                   "Condition for executing return statement must of type 'bool' not '%s'!",
                   condValType.name().c_str());
        
        condValIsBool = false;
    }
    
    // Verify the return type is correct, if it isn't then bail
    WC_GUARD(verifyReturnTypeForCurrentFunction(*this, returnedType));
    
    // Don't go any further if the condition value is not a boolean or is not valid
    WC_GUARD(condValIsBool && condVal.isValid());
    
    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a basic block for the return logic:
    std::string returnBBLbl = StringUtils::appendLineInfo("ReturnStmntWithCondVoid:return", astNode.getStartToken());
    llvm::BasicBlock * returnBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, returnBBLbl , parentFn);
    WC_ASSERT(returnBB);
    
    // Create a basic block for the continue logic:
    std::string continueBBLbl = StringUtils::appendLineInfo("ReturnStmntWithCondVoid:continue", astNode.getPastEndToken());
    llvm::BasicBlock * continueBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, "ReturnStmntWithCondVoid:continue", parentFn);
    WC_ASSERT(continueBB);
    
    // Now generate the code for the branch:
    if (astNode.isCondExprInversed()) {
        mCtx.mIRBuilder.CreateCondBr(condVal.mLLVMVal, continueBB, returnBB);
    }
    else {
        mCtx.mIRBuilder.CreateCondBr(condVal.mLLVMVal, returnBB, continueBB);
    }
    
    // Generate the code for the return:
    mCtx.mIRBuilder.SetInsertPoint(returnBB);
    mCtx.mIRBuilder.CreateRetVoid();
    
    // All further code is generated in the continue block:
    mCtx.mIRBuilder.SetInsertPoint(continueBB);
}

void Codegen::visit(const AST::ReturnStmntWithCondAndValue & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
