#include "WCFuncCall.hpp"

#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCFuncCallArgList.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool FuncCall::peek(const Token * currentToken) {
    return currentToken->type == TokenType::kLParen;
}

FuncCall * FuncCall::parse(ParseCtx & parseCtx) {
    // Must open with '('
    if (parseCtx.curTok->type != TokenType::kLParen) {
        parseCtx.error("Expected '(' !");
        return nullptr;
    }
    
    // Save and skip '('
    const Token * startToken = parseCtx.curTok;
    parseCtx.nextTok();
    
    // See if there is an arg list:
    FuncCallArgList * argList = nullptr;
    
    if (FuncCallArgList::peek(parseCtx.curTok)) {
        argList = FuncCallArgList::parse(parseCtx);
        WC_GUARD(argList, nullptr);
    }
    
    // Expect ')'
    if (parseCtx.curTok->type != TokenType::kRParen) {
        parseCtx.error("Expected ')' !");
        return nullptr;
    }
    
    // Save and skip ')'
    const Token * endToken = parseCtx.curTok;
    parseCtx.nextTok();
    
    // No args:
    return WC_NEW_AST_NODE(parseCtx, FuncCall, *startToken, argList, *endToken);
}

FuncCall::FuncCall(const Token & startToken, FuncCallArgList * argList, const Token & endToken) :
    mStartToken(startToken),
    mArgList(argList),
    mEndToken(endToken)
{
    if (mArgList) {
        mArgList->mParent = this;
    }
}

const Token & FuncCall::getStartToken() const {
    return mStartToken;
}

const Token & FuncCall::getEndToken() const {
    return mEndToken;
}

size_t FuncCall::numArgs() const {
    WC_GUARD(mArgList, 0);
    return mArgList->numArgs();
}

void FuncCall::getArgs(std::vector<AssignExpr*> & args) const {
    WC_GUARD(mArgList);
    mArgList->getArgs(args);
}

#warning FIXME - Codegen
#if 0
bool FuncCall::codegenArgsListExprs(CodegenCtx & cgCtx) {
    // If there is no args list then our job is easy, just return true for success
    WC_GUARD(mArgList, true);
    
    // Get the list of expressions for the args list:
    std::vector<AssignExpr*> args;
    mArgList->getArgs(args);
    mArgListExprsValues.reserve(args.size());
    
    // Evaluate the code for each arg:
    for (AssignExpr * expr : args) {
        // Generate the code, if it fails then bail
        llvm::Value * argValue = expr->codegenExprEval(cgCtx);
        WC_GUARD(argValue, false);

        // If the argument value requires storage then we need to make our own copy of it
        // prior to passing into the function. We also pass the pointer to the argument in
        // this case and not the raw value itself:
        DataType & exprDataType = expr->dataType();

        if (exprDataType.requiresStorage()) {
            // Value requires stack storage: need to alloc stack storage for it
            llvm::Value * argStackValue = cgCtx.irBuilder.CreateAlloca(argValue->getType());
            WC_ASSERT(argStackValue);

            // Store the value in this slot:
            llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(argValue, argStackValue);
            WC_ASSERT(storeInst);

            // The arg stack value is what gets passed in:
            mArgListExprsValues.push_back(argStackValue);
        }
        else {
            // Value does not require stack storage. Just pass normally:
            mArgListExprsValues.push_back(argValue);
        }
    }
    
    return true;    // All good!
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
