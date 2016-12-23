#include "WCTernaryExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCLOrExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// TernaryExpr
//-----------------------------------------------------------------------------
bool TernaryExpr::peek(const Token * tokenPtr) {
    return LOrExpr::peek(tokenPtr);
}

TernaryExpr * TernaryExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial expression
    LOrExpr * firstExpr = LOrExpr::parse(tokenPtr, alloc);
    WC_GUARD(firstExpr, nullptr);
    
    // See if a '?' follows:
    if (tokenPtr->type == TokenType::kQMark) {
        // Alright, consume that '?':
        ++tokenPtr;
        
        // Now parse the 'true' expression:
        AssignExpr * trueExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(trueExpr, nullptr);
        
        // Expect a colon to separate 'true' from false:
        if (tokenPtr->type != TokenType::kColon) {
            parseError(*tokenPtr, "Expected ':' following 'true' value in ternary expression.");
            return nullptr;
        }
        
        // Consume the ':'
        ++tokenPtr;
        
        // Now parse the 'false' expression:
        AssignExpr * falseExpr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(falseExpr, nullptr);
        
        // All done: return the parsed node:
        return WC_NEW_AST_NODE(alloc, TernaryExprWithCond, *firstExpr, *trueExpr, *falseExpr);
    }

    // Ternary expression with no condition:
    return WC_NEW_AST_NODE(alloc, TernaryExprNoCond, *firstExpr);
}

//-----------------------------------------------------------------------------
// TernaryExprNoCond
//-----------------------------------------------------------------------------
TernaryExprNoCond::TernaryExprNoCond(LOrExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & TernaryExprNoCond::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & TernaryExprNoCond::getEndToken() const {
    return mExpr.getEndToken();
}

bool TernaryExprNoCond::isLValue() {
    return mExpr.isLValue();
}

bool TernaryExprNoCond::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & TernaryExprNoCond::dataType() {
    return mExpr.dataType();
}

llvm::Value * TernaryExprNoCond::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * TernaryExprNoCond::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * TernaryExprNoCond::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// TernaryExprWithCond
//-----------------------------------------------------------------------------
TernaryExprWithCond::TernaryExprWithCond(LOrExpr & condExpr,
                                         AssignExpr & trueExpr,
                                         AssignExpr & falseExpr)
:
    mCondExpr(condExpr),
    mTrueExpr(trueExpr),
    mFalseExpr(falseExpr)
{
    mCondExpr.mParent = this;
    mTrueExpr.mParent = this;
    mFalseExpr.mParent = this;
}

const Token & TernaryExprWithCond::getStartToken() const {
    return mCondExpr.getStartToken();
}

const Token & TernaryExprWithCond::getEndToken() const {
    return mFalseExpr.getEndToken();
}

bool TernaryExprWithCond::isLValue() {
    return false;
}

bool TernaryExprWithCond::isConstExpr() {
    return  mCondExpr.isConstExpr() &&
            mTrueExpr.isConstExpr() &&
            mFalseExpr.isConstExpr();
}

DataType & TernaryExprWithCond::dataType() {
    // Assume the data type is that of the 'true' expression.
    // When we compile we will verify that the 'false' expression matches...
    return mTrueExpr.dataType();
}

llvm::Value * TernaryExprWithCond::codegenAddrOf(CodegenCtx & cgCtx) {
    // FIXME: Address of ternary expr: surely we should be able to do this?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of ternary expression result!");
    return nullptr;
}

llvm::Value * TernaryExprWithCond::codegenExprEval(CodegenCtx & cgCtx) {
    // Verify data types used by operator:
    //
    // FIXME: move this to after code generation, so we catch things like bad variable names better and
    // get better error info by delegating down the AST tree.
    WC_GUARD(compileCheckExprDataTypes(), nullptr);
    
    // Create a block for the true, false and follow on expressions:
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    llvm::BasicBlock * trueBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "TernaryExprWithCond:true", parentFn);
    llvm::BasicBlock * falseBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "TernaryExprWithCond:false", parentFn);
    llvm::BasicBlock * endBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "TernaryExprWithCond:end", parentFn);
    
    // Generate the code to evaluate the condition and branch to whatever block based on that:
    llvm::Value * condValue = mCondExpr.codegenExprEval(cgCtx);
    WC_GUARD(condValue, nullptr);
    cgCtx.irBuilder.CreateCondBr(condValue, trueBB, falseBB);
    
    // Generate the true expression code:
    cgCtx.irBuilder.SetInsertPoint(trueBB);
    llvm::Value * trueValue = mTrueExpr.codegenExprEval(cgCtx);
    WC_GUARD(trueValue, nullptr);
    cgCtx.irBuilder.CreateBr(endBB);
    
    // Generate the false expression code:
    cgCtx.irBuilder.SetInsertPoint(falseBB);
    llvm::Value * falseValue = mFalseExpr.codegenExprEval(cgCtx);
    WC_GUARD(falseValue, nullptr);
    cgCtx.irBuilder.CreateBr(endBB);
    
    // All code after that goes into the end block:
    cgCtx.irBuilder.SetInsertPoint(endBB);
    
    // Create a PHI node to merge the values:
    llvm::PHINode * phiNode = cgCtx.irBuilder.CreatePHI(trueValue->getType(), 2, "TernaryExprWithCond:PHI");
    WC_ASSERT(phiNode);
    phiNode->addIncoming(trueValue, trueBB);
    phiNode->addIncoming(falseValue, falseBB);
    
    // Return the PHI node as the value:
    return phiNode;
}

llvm::Constant * TernaryExprWithCond::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Verify data types used by operator:
    WC_GUARD(compileCheckExprDataTypes(), nullptr);
    
    // Generate the code for the boolean condition:
    llvm::Constant * condValue = mCondExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(condValue, nullptr);
    
    // See whether the value is true or false: use that to decide which sub expression to choose
    if (condValue->isZeroValue()) {
        return mFalseExpr.codegenExprConstEval(cgCtx);
    }
    
    return mTrueExpr.codegenExprConstEval(cgCtx);
}

bool TernaryExprWithCond::compileCheckExprDataTypes() const {
    const DataType & condType = mCondExpr.dataType();
    
    if (!condType.isBool()) {
        compileError("Condition expression for ternary operator must be of type 'bool', not '%s'!",
                     condType.name().c_str());
        
        return false;
    }
    
    const DataType & trueType = mTrueExpr.dataType();
    const DataType & falseType = mFalseExpr.dataType();
    
    if (!falseType.equals(trueType)) {
        compileError("'true' sub-expression must be the same type as the 'false' sub expression in ternary operator! "
                     "The type for the 'true' sub-expression was '%s'. "
                     "The type for the 'false' sub-expression was '%s'.",
                     trueType.name().c_str(),
                     falseType.name().c_str());
        
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
