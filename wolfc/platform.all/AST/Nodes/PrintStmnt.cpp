#include "PrintStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool PrintStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kPrint;
}

PrintStmnt * PrintStmnt::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    if (parseCtx.tok()->type != TokenType::kPrint) {
        parseCtx.error("Expected keyword 'print' for 'print()' statement!");
        return nullptr;
    }
    
    const Token * printTok = parseCtx.tok();
    parseCtx.nextTok();     // Consume 'print'
    
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("Expected '(' following 'print'!");
        return nullptr;
    }
    
    parseCtx.nextTok();     // Consume '('
    
    // Parse the inner expression
    AssignExpr * assignExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(assignExpr, nullptr);
    
    // Expect ')' following all that:
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("Expected closing ')' for 'print()' statement!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    const Token * closingParenTok = parseCtx.tok();
    parseCtx.nextTok();
    
    // Create and return the print statement
    return WC_NEW_AST_NODE(parseCtx, PrintStmnt, *printTok, *assignExpr, *closingParenTok);
}

PrintStmnt::PrintStmnt(const Token & startToken, AssignExpr & expr, const Token & endToken) :
    mStartToken(startToken),
    mExpr(expr),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

void PrintStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PrintStmnt::getStartToken() const {
    return mStartToken;
}

const Token & PrintStmnt::getEndToken() const {
    return mEndToken;
}

bool PrintStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
