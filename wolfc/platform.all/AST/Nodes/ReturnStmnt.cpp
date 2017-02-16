#include "ReturnStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
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
    return tokenPtr->type == TokenType::kReturn;
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
        parseCtx.nextTok();         // Consume 'if' or 'unless'
        parseCtx.skipNewlines();    // Skip any newlines that follow
        
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
            parseCtx.nextTok();         // Consume 'if' or 'unless'
            parseCtx.skipNewlines();    // Skip any newlines that follow
            
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

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
