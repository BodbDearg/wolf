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
    
    // See if assign expression follows:
    if (AssignExpr::peek(tokenPtr)) {
        // Return with a value:
        AssignExpr * expr = AssignExpr::parse(tokenPtr, alloc);
        WC_GUARD(expr, nullptr);
        return WC_NEW_AST_NODE(alloc, ReturnStmntWithValue, *returnToken, *expr);
    }
    
    // Return without a value:
    return WC_NEW_AST_NODE(alloc, ReturnStmntVoid, *returnToken);
}

ReturnStmnt::ReturnStmnt(const Token & startToken) : mStartToken(startToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & ReturnStmnt::getStartToken() const {
    return mStartToken;
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
                     parentFnReturnTy.name(),
                     parentFunc->name(),
                     returnTy.name());
        
        return false;
    }
    
    return true;    // All good!
}

//-----------------------------------------------------------------------------
// ReturnStmntVoid
//-----------------------------------------------------------------------------

ReturnStmntVoid::ReturnStmntVoid(const Token & startToken) : ReturnStmnt(startToken) {
    WC_EMPTY_FUNC_BODY();
}

const Token & ReturnStmntVoid::getEndToken() const {
    return mStartToken;
}

bool ReturnStmntVoid::codegen(CodegenCtx & cgCtx) {
    // Verify the return type is correct firstly
    WC_GUARD(verifyReturnTypeCorrect(), false);
    
    // Generate the code for the return and return true for success
    cgCtx.irBuilder.CreateRetVoid();
    return true;
}

const DataType & ReturnStmntVoid::dataType() const {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kVoid);
}

//-----------------------------------------------------------------------------
// ReturnStmntWithValue
//-----------------------------------------------------------------------------

ReturnStmntWithValue::ReturnStmntWithValue(const Token & startToken, AssignExpr & expr) :
    ReturnStmnt(startToken),
    mExpr(expr)
{
    mExpr.mParent = this;
}

const Token & ReturnStmntWithValue::getEndToken() const {
    return mExpr.getEndToken();
}

bool ReturnStmntWithValue::codegen(CodegenCtx & cgCtx) {
    // Verify the return type is correct firstly
    WC_GUARD(verifyReturnTypeCorrect(), false);
    
    // Codegen the assign expression for the return
    llvm::Value * exprResult = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(exprResult, false);
    
    // Now generate the return and return true for success
    cgCtx.irBuilder.CreateRet(exprResult);
    return true;
}

const DataType & ReturnStmntWithValue::dataType() const {
    return mExpr.dataType();
}

WC_END_NAMESPACE
