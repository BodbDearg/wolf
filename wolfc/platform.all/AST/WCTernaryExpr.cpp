#include "WCTernaryExpr.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCOrExpr.hpp"
#include "WCToken.hpp"

#warning TODO: remove
#include "WCModule.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// TernaryExpr
//-----------------------------------------------------------------------------

bool TernaryExpr::peek(const Token * tokenPtr) {
    return OrExpr::peek(tokenPtr);
}

TernaryExpr * TernaryExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the initial expression
    OrExpr * firstExpr = OrExpr::parse(tokenPtr, alloc);
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

TernaryExprNoCond::TernaryExprNoCond(OrExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & TernaryExprNoCond::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & TernaryExprNoCond::getEndToken() const {
    return mExpr.getEndToken();
}

bool TernaryExprNoCond::isLValue() const {
    return mExpr.isLValue();
}

const DataType & TernaryExprNoCond::dataType() const {
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

TernaryExprWithCond::TernaryExprWithCond(OrExpr & condExpr,
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

bool TernaryExprWithCond::isLValue() const {
    return false;
}

const DataType & TernaryExprWithCond::dataType() const {
    // Assume the data type is that of the 'true' expression.
    // When we compile we will verify that the 'false' expression matches...
    return mTrueExpr.dataType();
}

llvm::Value * TernaryExprWithCond::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of an expression that is not an lvalue!");
    return nullptr;
}

llvm::Value * TernaryExprWithCond::codegenExprEval(CodegenCtx & cgCtx) {
    // Verify data types used by operator:
    compileCheckExprDataTypes();
    
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
    compileCheckExprDataTypes();
    
    #warning TODO: Handle constant evaluation of ternary expression
    compileError("Can't do constant evaluation of ternary expression currently!");
    return nullptr;
}

bool TernaryExprWithCond::compileCheckExprDataTypes() const {
    const DataType & condType = mCondExpr.dataType();
    
    if (!condType.isBool()) {
        compileError("Condition expression for ternary operator must be of type 'bool', not '%s'!", condType.name());
        return false;
    }
    
    const DataType & trueType = mTrueExpr.dataType();
    const DataType & falseType = mFalseExpr.dataType();
    
    if (!falseType.equals(trueType)) {
        compileError("'true' sub-expression must be the same type as the 'false' sub expression in ternary operator! "
                     "The type for the 'true' sub-expression was '%s'. "
                     "The type for the 'false' sub-expression was '%s'.",
                     trueType.name(),
                     falseType.name());
        
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
