#include "WCReturnStmnt.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCFunc.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCToken.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/* Utility. Tell if a token is a condition token type. */
static bool isCondTokenType(TokenType tokenType) {
    return tokenType == TokenType::kIf || tokenType == TokenType::kUnless;
}

//-----------------------------------------------------------------------------
// ReturnStmnt
//-----------------------------------------------------------------------------

bool ReturnStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kReturn;
}

ReturnStmnt * ReturnStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected return statement!");
        return nullptr;
    }
    
    // Consume 'return' token
    const Token * returnToken = tokenPtr;
    ++tokenPtr;
    
    // See if a condition token follows:
    if (isCondTokenType(tokenPtr->type)) {
        // Save the 'if' or 'unless' token and skip
        const Token * condToken = tokenPtr;
        ++tokenPtr;
        
        // Parse the assign expression that follows:
        AssignExpr * condExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(condExpr, nullptr);
        
        // Conditional 'return' without a value:
        return WC_NEW_AST_NODE(alloc,
                               ReturnStmntWithCondVoid,
                               *returnToken,
                               *condToken,
                               *condExpr);
    }
    
    // See if assign expression follows:
    if (AssignExpr::peek(tokenPtr)) {
        // Parse the assign expression for the return value:
        AssignExpr * returnExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(returnExpr, nullptr);
        
        // See if a condition token follows:
        if (isCondTokenType(tokenPtr->type)) {
            // Save the 'if' or 'unless' token and skip
            const Token * condToken = tokenPtr;
            ++tokenPtr;
            
            // Parse the assign expression that follows:
            AssignExpr * condExpr = AssignExpr::parse(tokenPtr, alloc);
            WC_GUARD(condExpr, nullptr);
            
            // Conditional 'return' with a value:
            return WC_NEW_AST_NODE(alloc,
                                   ReturnStmntWithCondAndValue,
                                   *returnToken,
                                   *returnExpr,
                                   *condToken,
                                   *condExpr);
        }
        
        // Non conditional 'return' with a return value:
        return WC_NEW_AST_NODE(alloc, ReturnStmntNoCondWithValue, *returnToken, *returnExpr);
    }
    
    // Non conditional 'return' without a value:
    return WC_NEW_AST_NODE(alloc, ReturnStmntNoCondVoid, *returnToken);
}

ReturnStmnt::ReturnStmnt(const Token & returnToken) : mReturnToken(returnToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & ReturnStmnt::getStartToken() const {
    return mReturnToken;
}

bool ReturnStmnt::verifyReturnTypeCorrect() const {
    // Get the parent function:
    const Func * parentFunc = firstParentOfType<Func>();
    
    if (!parentFunc) {
        compileError("Unable to determine parent function for return statement!");
        return false;
    }
    
    // TODO: handle auto promotion of types here
    // Verify correctness:
    const DataType & parentFnReturnTy = parentFunc->returnDataType();
    const DataType & returnTy = dataType();
    
    if (!returnTy.equals(parentFnReturnTy)) {
        compileError("Invalid return statement! Need to return type '%s' for function '%s' "
                     "instead of type '%s'!",
                     parentFnReturnTy.name().c_str(),
                     parentFunc->name(),
                     returnTy.name().c_str());
        
        return false;
    }
    
    return true;    // All good!
}

//-----------------------------------------------------------------------------
// ReturnStmntNoCondVoid
//-----------------------------------------------------------------------------

ReturnStmntNoCondVoid::ReturnStmntNoCondVoid(const Token & returnToken) : ReturnStmnt(returnToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & ReturnStmntNoCondVoid::getEndToken() const {
    return mReturnToken;
}

bool ReturnStmntNoCondVoid::codegen(CodegenCtx & cgCtx) {
    // Verify the return type is correct firstly
    WC_GUARD(verifyReturnTypeCorrect(), false);
    
    // Generate the code for the return and return true for success
    cgCtx.irBuilder.CreateRetVoid();
    return true;
}

const DataType & ReturnStmntNoCondVoid::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kVoid);
}

//-----------------------------------------------------------------------------
// ReturnStmntNoCondWithValue
//-----------------------------------------------------------------------------

ReturnStmntNoCondWithValue::ReturnStmntNoCondWithValue(const Token & returnToken, AssignExpr & returnExpr) :
    ReturnStmnt(returnToken),
    mReturnExpr(returnExpr)
{
    mReturnExpr.mParent = this;
}

const Token & ReturnStmntNoCondWithValue::getEndToken() const {
    return mReturnExpr.getEndToken();
}

bool ReturnStmntNoCondWithValue::codegen(CodegenCtx & cgCtx) {
    // Verify the return type is correct firstly
    WC_GUARD(verifyReturnTypeCorrect(), false);
    
    // Codegen the assign expression for the return
    llvm::Value * returnExprResult = mReturnExpr.codegenExprEval(cgCtx);
    WC_GUARD(returnExprResult, false);
    
    // Now generate the return and return true for success
    cgCtx.irBuilder.CreateRet(returnExprResult);
    return true;
}

const DataType & ReturnStmntNoCondWithValue::dataType() const {
    return mReturnExpr.dataType();
}

//-----------------------------------------------------------------------------
// ReturnStmntWithCondBase
//-----------------------------------------------------------------------------

ReturnStmntWithCondBase::ReturnStmntWithCondBase(const Token & returnToken,
                                                 const Token & condToken,
                                                 AssignExpr & condExpr)
:
    ReturnStmnt(returnToken),
    mCondToken(condToken),
    mCondExpr(condExpr)
{
    mCondExpr.mParent = this;
}

const Token & ReturnStmntWithCondBase::getEndToken() const {
    return mCondExpr.getEndToken();
}

bool ReturnStmntWithCondBase::isCondExprInversed() const {
    return mCondToken.type == TokenType::kUnless;
}

//-----------------------------------------------------------------------------
// ReturnStmntWithCondVoid
//-----------------------------------------------------------------------------

ReturnStmntWithCondVoid::ReturnStmntWithCondVoid(const Token & returnToken,
                                                 const Token & condToken,
                                                 AssignExpr & condExpr)
:
    ReturnStmntWithCondBase(returnToken, condToken, condExpr)
{
    WC_EMPTY_FUNC_BODY();
}
    
bool ReturnStmntWithCondVoid::codegen(CodegenCtx & cgCtx) {
    // Verify the return type is correct firstly
    WC_GUARD(verifyReturnTypeCorrect(), false);
    
    // The conditional expression for returning must be void
    if (!mCondExpr.dataType().isBool()) {
        compileError("Condition for 'return' statement must evaluate to type 'bool' not '%s'!",
                     mCondExpr.dataType().name().c_str());
        
        return false;
    }
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a basic block for the return logic:
    mReturnBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ReturnStmntWithCondVoid:return", parentFn);
    WC_ASSERT(mReturnBlock);
    
    // Create a basic block for the continue logic:
    mContinueBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ReturnStmntWithCondVoid:continue", parentFn);
    WC_ASSERT(mContinueBlock);
    
    // Evaluate the condition:
    llvm::Value * condValue = mCondExpr.codegenExprEval(cgCtx);
    WC_GUARD(condValue, nullptr);
    
    // Now generate the code for the branch:
    if (isCondExprInversed()) {
        cgCtx.irBuilder.CreateCondBr(condValue, mContinueBlock, mReturnBlock);
    }
    else {
        cgCtx.irBuilder.CreateCondBr(condValue, mReturnBlock, mContinueBlock);
    }
    
    // Generate the code for the return:
    cgCtx.irBuilder.SetInsertPoint(mReturnBlock);
    cgCtx.irBuilder.CreateRetVoid();
    
    // All further code is generated in the continue block:
    cgCtx.irBuilder.SetInsertPoint(mContinueBlock);
    return true;
}
    
const DataType & ReturnStmntWithCondVoid::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kVoid);
}

//-----------------------------------------------------------------------------
// ReturnStmntWithCondAndValue
//-----------------------------------------------------------------------------

ReturnStmntWithCondAndValue::ReturnStmntWithCondAndValue(const Token & returnToken,
                                                         AssignExpr & returnExpr,
                                                         const Token & condToken,
                                                         AssignExpr & condExpr)
:
    ReturnStmntWithCondBase(returnToken,
                            condToken,
                            condExpr),
    mReturnExpr(returnExpr)
{
    mReturnExpr.mParent = this;
}
    
bool ReturnStmntWithCondAndValue::codegen(CodegenCtx & cgCtx) {
    // Verify the return type is correct firstly
    WC_GUARD(verifyReturnTypeCorrect(), false);
    
    // The conditional expression for returning must be void
    if (!mCondExpr.dataType().isBool()) {
        compileError("Condition for 'return' statement must evaluate to type 'bool' not '%s'!",
                     mCondExpr.dataType().name().c_str());
        
        return false;
    }
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create a basic block for the return logic:
    mReturnBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ReturnStmntWithCondVoid:return", parentFn);
    WC_ASSERT(mReturnBlock);
    
    // Create a basic block for the continue logic:
    mContinueBlock = llvm::BasicBlock::Create(cgCtx.llvmCtx, "ReturnStmntWithCondVoid:continue", parentFn);
    WC_ASSERT(mContinueBlock);
    
    // Evaluate the condition:
    llvm::Value * condValue = mCondExpr.codegenExprEval(cgCtx);
    WC_GUARD(condValue, nullptr);
    
    // Now generate the code for the branch:
    if (isCondExprInversed()) {
        cgCtx.irBuilder.CreateCondBr(condValue, mContinueBlock, mReturnBlock);
    }
    else {
        cgCtx.irBuilder.CreateCondBr(condValue, mReturnBlock, mContinueBlock);
    }
    
    // Begin generating code for the return case, switch to that block:
    cgCtx.irBuilder.SetInsertPoint(mReturnBlock);
    
    // Codegen the assign expression for the return
    llvm::Value * returnExprResult = mReturnExpr.codegenExprEval(cgCtx);
    WC_GUARD(returnExprResult, false);
    
    // Generate the code for the return:
    cgCtx.irBuilder.CreateRet(returnExprResult);
    
    // All further code is generated in the continue block:
    cgCtx.irBuilder.SetInsertPoint(mContinueBlock);
    return true;
}
    
const DataType & ReturnStmntWithCondAndValue::dataType() const {
    return mReturnExpr.dataType();
}

WC_END_NAMESPACE
