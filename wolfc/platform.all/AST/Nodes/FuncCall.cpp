#include "FuncCall.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool FuncCall::peek(const Token * currentToken) {
    return currentToken->type == TokenType::kLParen;
}

FuncCall * FuncCall::parse(ParseCtx & parseCtx) {
    // Must open with '('
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("Expected '(' !");
        return nullptr;
    }
    
    // Save and skip '('
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Start parsing the arg list
    std::vector<AssignExpr*> argList;
    
    while (AssignExpr::peek(parseCtx.tok())) {
        // Parse the arg and save if it was parsed ok
        AssignExpr * argExpr = AssignExpr::parse(parseCtx);
        
        if (argExpr) {
            argList.push_back(argExpr);
        }
        
        // If a comma does not follow then we are done
        if (parseCtx.tok()->type != TokenType::kComma) {
            break;
        }
        
        // Otherwise consume the comma
        parseCtx.nextTok();
    }
    
    // Expect ')'
    const Token * endToken = parseCtx.tok();
    
    if (endToken->type != TokenType::kRParen) {
        parseCtx.error("Expected ')' !");
    }
    else {
        // Skip the ')'
        parseCtx.nextTok();
    }
    
    // All good, return the node
    return WC_NEW_AST_NODE(parseCtx, FuncCall, *startToken, argList, *endToken);
}

FuncCall::FuncCall(const Token & startToken,
                   const std::vector<AssignExpr*> & args,
                   const Token & endToken)
:
    mStartToken(startToken),
    mEndToken(endToken),
    mArgs()
{
    mArgs.reserve(args.size());
    
    for (AssignExpr * arg : args) {
        arg->mParent = this;
        mArgs.push_back(arg);
    }
}

void FuncCall::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & FuncCall::getStartToken() const {
    return mStartToken;
}

const Token & FuncCall::getEndToken() const {
    return mEndToken;
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
