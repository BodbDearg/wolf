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
    #warning Handle newlines during parsing
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

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
