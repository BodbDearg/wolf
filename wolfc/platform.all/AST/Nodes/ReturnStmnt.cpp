#include "ReturnStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Assert.hpp"
#include "AssignExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "Func.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

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

ReturnStmnt * ReturnStmnt::parse(ParseCtx & parseCtx) {
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected return statement!");
        return nullptr;
    }
    
    // Consume 'return' token
    const Token * returnToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // See if a condition token follows:
    if (isCondTokenType(parseCtx.tok()->type)) {
        // Save the 'if' or 'unless' token and skip
        const Token * condToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Parse the assign expression that follows:
        AssignExpr * condExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(condExpr, nullptr);
        
        // Conditional 'return' without a value:
        return WC_NEW_AST_NODE(parseCtx,
                               ReturnStmntWithCondVoid,
                               *returnToken,
                               *condToken,
                               *condExpr);
    }
    
    // See if assign expression follows:
    if (AssignExpr::peek(parseCtx.tok())) {
        // Parse the assign expression for the return value:
        AssignExpr * returnExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(returnExpr, nullptr);
        
        // See if a condition token follows:
        if (isCondTokenType(parseCtx.tok()->type)) {
            // Save the 'if' or 'unless' token and skip
            const Token * condToken = parseCtx.tok();
            parseCtx.nextTok();
            
            // Parse the assign expression that follows:
            AssignExpr * condExpr = AssignExpr::parse(parseCtx);
            WC_GUARD(condExpr, nullptr);
            
            // Conditional 'return' with a value:
            return WC_NEW_AST_NODE(parseCtx,
                                   ReturnStmntWithCondAndValue,
                                   *returnToken,
                                   *returnExpr,
                                   *condToken,
                                   *condExpr);
        }
        
        // Non conditional 'return' with a return value:
        return WC_NEW_AST_NODE(parseCtx, ReturnStmntNoCondWithValue, *returnToken, *returnExpr);
    }
    
    // Non conditional 'return' without a value:
    return WC_NEW_AST_NODE(parseCtx, ReturnStmntNoCondVoid, *returnToken);
}

ReturnStmnt::ReturnStmnt(const Token & returnToken) : mReturnToken(returnToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & ReturnStmnt::getStartToken() const {
    return mReturnToken;
}

//-----------------------------------------------------------------------------
// ReturnStmntNoCondVoid
//-----------------------------------------------------------------------------
ReturnStmntNoCondVoid::ReturnStmntNoCondVoid(const Token & returnToken) : ReturnStmnt(returnToken) {
    WC_EMPTY_FUNC_BODY();
}

void ReturnStmntNoCondVoid::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ReturnStmntNoCondVoid::getEndToken() const {
    return mReturnToken;
}

const DataType & ReturnStmntNoCondVoid::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kVoid);
}

bool ReturnStmntNoCondVoid::allCodepathsHaveUncondRet() const {
    return true;
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

void ReturnStmntNoCondWithValue::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ReturnStmntNoCondWithValue::getEndToken() const {
    return mReturnExpr.getEndToken();
}

#warning FIXME - Codegen
#if 0
bool ReturnStmntNoCondWithValue::codegen(CodegenCtx & cgCtx) {
    // Codegen the return type and verify the return type is correct
    WC_GUARD(codegenAndVerifyReturnDataType(cgCtx), false);
    
    // Codegen the assign expression for the return
    llvm::Value * returnExprResult = mReturnExpr.codegenExprEval(cgCtx);
    WC_GUARD(returnExprResult, false);
    
    // Now generate the return and return true for success
    cgCtx.irBuilder.CreateRet(returnExprResult);
    
    // Create a new basic block for the unreachable code past this and set all future code to insert after it:
    {
        Func * parentFunc = getParentFunc();
        WC_ASSERT(parentFunc);
        std::string unreachableBBLbl = makeLLVMLabelForTok("ReturnStmntNoCondWithValue:unreachable", getEndToken());
        llvm::BasicBlock * unreachableBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, unreachableBBLbl, parentFunc->mLLVMFunc);
        WC_ASSERT(unreachableBB);
        cgCtx.irBuilder.SetInsertPoint(unreachableBB);
        cgCtx.irBuilder.CreateUnreachable();
    }
    
    return true;    // Success!
}
#endif

const DataType & ReturnStmntNoCondWithValue::dataType() {
    return mReturnExpr.dataType();
}

bool ReturnStmntNoCondWithValue::allCodepathsHaveUncondRet() const {
    return true;
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

bool ReturnStmntWithCondBase::allCodepathsHaveUncondRet() const {
    return false;
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

void ReturnStmntWithCondVoid::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

#warning FIXME - Codegen
#if 0
bool ReturnStmntWithCondVoid::codegen(CodegenCtx & cgCtx) {
    // Codegen the return type and verify the return type is correct
    WC_GUARD(codegenAndVerifyReturnDataType(cgCtx), false);
    
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
#endif
    
const DataType & ReturnStmntWithCondVoid::dataType() {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kVoid);
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

void ReturnStmntWithCondAndValue::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

#warning FIXME - Codegen
#if 0
bool ReturnStmntWithCondAndValue::codegen(CodegenCtx & cgCtx) {
    // Codegen the return type and verify the return type is correct
    WC_GUARD(codegenAndVerifyReturnDataType(cgCtx), false);
    
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
#endif
    
const DataType & ReturnStmntWithCondAndValue::dataType() {
    return mReturnExpr.dataType();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
