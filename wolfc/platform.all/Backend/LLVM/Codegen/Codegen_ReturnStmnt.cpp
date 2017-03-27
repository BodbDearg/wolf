//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/ReturnStmnt.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/FuncDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "DataType/Types/VoidDataType.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/**
 * Get the compiled return type for the current function.
 * Issues an error if that fails to happen (should never happen if compiler has no bugs related to this).
 */
static CompiledDataType getReturnTypeCDTForCurrentFunc(Codegen & cg) {
    // This should never be called when there isn't a valid function!
    if (!cg.mCtx.mCurFunc) {
        cg.mCtx.error("Internal error! Can't determine the return type for the current function because "
                      "there is no current function set! This is a compiler bug!");
        
        return CompiledDataType();
    }
    
    // Get the compiled type for the current function
    const DataType & curFuncAbstractDataType = cg.mCtx.mCurFunc->mCompiledType.getDataType();
    
    if (curFuncAbstractDataType.getTypeId() != DataTypeId::kFunc) {
        cg.mCtx.error("Internal error! Can't determine the return type for the current function because "
                      "the data type for the current function is not set to be a function. "
                      "This is a compiler bug!");
        
        return CompiledDataType();
    }
    
    // Alright, cast to the function data type and compile the return type
    const FuncDataType & funcDataType = static_cast<const FuncDataType&>(curFuncAbstractDataType);
    funcDataType.mReturnType.accept(cg.mCodegenDataType);
    return cg.mCtx.popCompiledDataType();
}

/**
 * Verify that the data type returned is type correct for the current function.
 * Returns true if verification is okay, false otherwise.
 */
static bool verifyReturnTypeOkForCurrentFunc(Codegen & cg,
                                             const CompiledDataType & returnValTypeCDT)
{
    // Get the return type for the current function. If invalid then return type is not okay.
    // Don't issue an error if it is invalid since we'd have already done that:
    CompiledDataType funcRetTypeCDT = getReturnTypeCDTForCurrentFunc(cg);
    WC_GUARD(funcRetTypeCDT.isValid(), false);
    
    // Now check that the types match, if they don't then return false for failure:
    const DataType & funcRetType = funcRetTypeCDT.getDataType();
    const DataType & returnValType = returnValTypeCDT.getDataType();
    
    if (!funcRetType.equals(returnValType)) {
        // Note: don't issue an error in the case of one of these being undefined since we'd have
        // already done that elsewhere:
        if (!funcRetType.isUndefined() && !returnValType.isUndefined()) {
            cg.mCtx.error("Invalid return type for current function! Function expects return type '%s' "
                          "but instead return statement returns type '%s'!",
                          funcRetType.name().c_str(),
                          returnValType.name().c_str());
        }
        
        // Failed:
        return false;
    }
    
    // All good!
    return true;
}

/**
 * Verify that the value returned is type correct for the current function.
 * Will attempt to implicit cast the value if possible.
 * Returns true if verification and/or implicit casting went okay, false otherwise.
 */
static bool verifyAndAutoPromoteReturnValForCurrentFunc(Codegen & cg,
                                                        Value & valReturned)
{
    // Get the return type for the current function. If invalid then the return value is not okay.
    // Don't issue an error if it is invalid since we'd have already done that:
    CompiledDataType funcRetTypeCDT = getReturnTypeCDTForCurrentFunc(cg);
    WC_GUARD(funcRetTypeCDT.isValid(), false);
    
    // Implicitly cast the value to the return type if possible:
    ImplicitCasts::castSingleValueIfRequired(cg, valReturned, funcRetTypeCDT);
    
    // Now return whether the type of the return value is valid for this function:
    return verifyReturnTypeOkForCurrentFunc(cg, valReturned.mCompiledType);
}

void Codegen::visit(const AST::ReturnStmntNoCondVoid & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the void return type
    PrimitiveDataTypes::getVoidDataType().accept(mCodegenDataType);
    CompiledDataType voidCDT = mCtx.popCompiledDataType();
    
    // Verify the return type is correct, if it isn't then bail
    WC_GUARD(verifyReturnTypeOkForCurrentFunc(*this, voidCDT));
    
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
    
    // Verify the type of the return value is correct for this function and try to implicitly
    // cast if it is mismatched to correct it. If we fail in this, then bail out...
    WC_GUARD(verifyAndAutoPromoteReturnValForCurrentFunc(*this, returnVal));
    
    // Make sure we have a valid value to return.
    // Note: do not expect this value to require a load either.
    WC_GUARD(returnVal.isValid());
    WC_ASSERT(!returnVal.mRequiresLoad);
    
    // Now generate the return
    if (returnVal.isVoid()) {
        mCtx.mIRBuilder.CreateRetVoid();
    }
    else {
        WC_ASSERT(returnVal.mLLVMVal);
        mCtx.mIRBuilder.CreateRet(returnVal.mLLVMVal);
    }
    
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
    CompiledDataType voidCDT = mCtx.popCompiledDataType();

    // Codegen the conditional expression
    astNode.mCondExpr.accept(*this);
    Value condVal = mCtx.popValue();
    
    // Codegen the bool data type and try to auto promote the condition expr to bool if it isn't already
    PrimitiveDataTypes::getBoolDataType().accept(mCodegenDataType);
    CompiledDataType boolCDT = mCtx.popCompiledDataType();
    ImplicitCasts::castSingleValueIfRequired(*this, condVal, boolCDT);
    
    // Verify that the condition value is a bool
    const DataType & condValType = condVal.mCompiledType.getDataType();
    bool condValIsBool = true;
    
    if (!condValType.isBool()) {
        // Note: no errors in the case of 'undefined' since they would have been raised elsewhere
        if (!condValType.isUndefined()) {
            mCtx.error(astNode.mCondExpr,
                       "Condition for executing return statement must of type 'bool' not '%s'!",
                       condValType.name().c_str());
        }
        
        condValIsBool = false;
    }
    
    // Verify the return type is correct, if it isn't then bail
    WC_GUARD(verifyReturnTypeOkForCurrentFunc(*this, voidCDT));
    
    // Don't go any further if these checks do not pass
    WC_GUARD(condValIsBool);
    WC_GUARD(condVal.isValid());
    
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
    WC_ASSERT(condVal.mLLVMVal);
    
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
    
    // Codegen the conditional expression
    astNode.mCondExpr.accept(*this);
    Value condVal = mCtx.popValue();
    
    // Codegen the bool data type and try to auto promote the condition expr to bool if it isn't already
    PrimitiveDataTypes::getBoolDataType().accept(mCodegenDataType);
    CompiledDataType boolCDT = mCtx.popCompiledDataType();
    ImplicitCasts::castSingleValueIfRequired(*this, condVal, boolCDT);
    
    // Verify that the condition value is a bool
    const DataType & condValType = condVal.mCompiledType.getDataType();
    bool condValIsBool = true;
    
    if (!condValType.isBool()) {
        // Note: no errors in the case of 'undefined' since they would have been raised elsewhere
        if (!condValType.isUndefined()) {
            mCtx.error(astNode.mCondExpr,
                       "Condition for executing return statement must of type 'bool' not '%s'!",
                       condValType.name().c_str());
        }
        
        condValIsBool = false;
    }
    
    // Grab the parent function
    llvm::Function * parentFn = mCtx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a basic block for the return logic:
    std::string returnBBLbl = StringUtils::appendLineInfo("ReturnStmntWithCondAndValue:return", astNode.getStartToken());
    llvm::BasicBlock * returnBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, returnBBLbl, parentFn);
    WC_ASSERT(returnBB);
    
    // Create a basic block for the continue logic:
    std::string continueBBLbl = StringUtils::appendLineInfo("ReturnStmntWithCondAndValue:continue", astNode.getPastEndToken());
    llvm::BasicBlock * continueBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, continueBBLbl, parentFn);
    WC_ASSERT(continueBB);
    
    // Begin generating code for the return case, switch to that block:
    mCtx.pushInsertBlock();
    mCtx.mIRBuilder.SetInsertPoint(returnBB);
    
    // Codegen the value to return
    astNode.mReturnExpr.accept(*this);
    Value returnVal = mCtx.popValue();
    
    // Verify the type of the return value is correct for this function and try to implicitly
    // cast if it is mismatched to correct it. If we fail in this, then bail out...
    WC_GUARD(verifyAndAutoPromoteReturnValForCurrentFunc(*this, returnVal));
    
    // Make sure we have a valid value to return.
    // Note: do not expect this value to require a load either.
    WC_GUARD(returnVal.isValid());
    WC_ASSERT(!returnVal.mRequiresLoad);
    
    // Generate the code for the return:
    if (returnVal.isVoid()) {
        mCtx.mIRBuilder.CreateRetVoid();
    }
    else {
        WC_ASSERT(returnVal.mLLVMVal);
        mCtx.mIRBuilder.CreateRet(returnVal.mLLVMVal);
    }
    
    // Go back to the block where the branch is in
    mCtx.popInsertBlock();
    
    // Don't go any further if these checks do not pass
    WC_GUARD(condValIsBool);
    WC_GUARD(condVal.isValid());
    
    // Now generate the code for the branch:
    WC_ASSERT(condVal.mLLVMVal);
    
    if (astNode.isCondExprInversed()) {
        mCtx.mIRBuilder.CreateCondBr(condVal.mLLVMVal, continueBB, returnBB);
    }
    else {
        mCtx.mIRBuilder.CreateCondBr(condVal.mLLVMVal, returnBB, continueBB);
    }
    
    // All further code is generated in the continue block:
    mCtx.mIRBuilder.SetInsertPoint(continueBB);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
