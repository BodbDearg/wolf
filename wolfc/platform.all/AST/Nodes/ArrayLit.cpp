#include "ArrayLit.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool ArrayLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kLBrack;
}

ArrayLit * ArrayLit::parse(ParseCtx & parseCtx) {
    // Parse the initial '[':
    if (parseCtx.tok()->type != TokenType::kLBrack) {
        parseCtx.error("Expected integer literal!");
        return nullptr;
    }
    
    const Token * lBrack = parseCtx.tok();
    parseCtx.nextTok();
    
    // Start parsing the list of sub expressions
    std::vector<AssignExpr*> exprs;
    
    while (AssignExpr::peek(parseCtx.tok())) {
        // Parse the expression and save if it was parsed ok
        AssignExpr * expr = AssignExpr::parse(parseCtx);
        
        if (expr) {
            exprs.push_back(expr);
        }
        
        // If a comma does not follow then we are done
        if (parseCtx.tok()->type != TokenType::kComma) {
            break;
        }
        
        // Otherwise consume the comma
        parseCtx.nextTok();
    }
    
    // Parse the closing ']'
    if (parseCtx.tok()->type != TokenType::kRBrack) {
        parseCtx.error("Expected closing ']' for array literal!");
        return nullptr;
    }
    
    const Token * rBrack = parseCtx.tok();
    parseCtx.nextTok();
    
    // Now return the parsed node:
    return WC_NEW_AST_NODE(parseCtx, ArrayLit, *lBrack, exprs, *rBrack);
}

ArrayLit::ArrayLit(const Token & lBrack,
                   const std::vector<AssignExpr*> & exprs,
                   const Token & rBrack)
:
    mLBrack(lBrack),
    mExprs(),
    mRBrack(rBrack)
{
    mExprs.reserve(exprs.size());
    
    for (AssignExpr * expr : exprs) {
        expr->mParent = this;
        mExprs.push_back(expr);
    }
}

void ArrayLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & ArrayLit::getStartToken() const {
    return mLBrack;
}

const Token & ArrayLit::getEndToken() const {
    return mRBrack;
}

#warning FIXME - Codegen
#if 0
llvm::Value * ArrayLit::codegenAddrOf(CodegenCtx & cgCtx) {
    // If already done then just return the previous calculated result
    WC_GUARD(!mAddrOfResult, mAddrOfResult);
    
    // Make sure the expression was evaluated
    codegenExprEval(cgCtx);
    WC_GUARD(mExprEvalResult, nullptr);
    
    // Create an alloc to hold the result of the function call.
    // This will be what we return:
    mAddrOfResult = cgCtx.irBuilder.CreateAlloca(dataType().mLLVMType);
    WC_ASSERT(mAddrOfResult);
    
    // Store the function call result in
    cgCtx.irBuilder.CreateStore(mExprEvalResult, mAddrOfResult);
    
    // Return the result
    return mAddrOfResult;
}

llvm::Value * ArrayLit::codegenExprEval(CodegenCtx & cgCtx) {
    // If already done then just return the previous calculated result
    WC_GUARD(!mExprEvalResult, mExprEvalResult);
    
    // Generate the code for the element type:
    WC_GUARD(codegenLLVMType(cgCtx), nullptr);
    
    // Allocate stack space for the array:
    mStorage = cgCtx.irBuilder.CreateAlloca(mDataType->mLLVMType);
    WC_ASSERT(mStorage);
    
    // Evaluate the array element expressions:
    std::vector<AssignExpr*> exprs;
    mExprs.getExprs(exprs);
    std::vector<llvm::Value*> exprValues;
    exprValues.reserve(exprs.size());
    
    for (size_t i = 0; i < exprs.size(); ++i) {
        // Codegen the expression:
        llvm::Value * exprValue = exprs[i]->codegenExprEval(cgCtx);
        WC_GUARD(exprValue, nullptr);
        exprValues.push_back(exprValue);
    }
    
    // Assign each of the element expressions to the array:
    for (size_t i = 0; i < exprs.size(); ++i) {
        // Figure out the pointer to the array element:
        llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), 0);
        WC_ASSERT(zeroIndex);
        llvm::ConstantInt * arrayIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), i);
        WC_ASSERT(arrayIndex);
        llvm::Value * arrayElemPtr = cgCtx.irBuilder.CreateGEP(mStorage, { zeroIndex, arrayIndex });
        WC_ASSERT(arrayElemPtr);
        
        // TODO: this probably will not work for complex types and nested arrays
        // Do the actual store to the array element.
        llvm::Value * storeResult = cgCtx.irBuilder.CreateStore(exprValues[i], arrayElemPtr);
        WC_ASSERT(storeResult);
    }
    
    mExprEvalResult = cgCtx.irBuilder.CreateLoad(mStorage);
    return mExprEvalResult;
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
