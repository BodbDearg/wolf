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

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
