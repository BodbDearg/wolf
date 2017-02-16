#include "VarDecl.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "Identifier.hpp"
#include "LinearAlloc.hpp"
#include "Type.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// VarDecl
//-----------------------------------------------------------------------------
bool VarDecl::peek(const Token * tokenPtr) {
    #warning Handle newlines during parsing
    return tokenPtr->type == TokenType::kLet;
}

VarDecl * VarDecl::parse(ParseCtx & parseCtx) {
    // Parse 'let' keyword
    if (parseCtx.tok()->type != TokenType::kLet) {
        parseCtx.error("Expected keyword 'let' at start of a variable declaration!");
        return nullptr;
    }
    
    // Save and skip 'let'
    const Token * varToken = parseCtx.tok();
    parseCtx.nextTok();         // Consume 'let'
    parseCtx.skipNewlines();    // Skip any newlines that follow
    
    // Parse the identifier ahead
    Identifier * ident = Identifier::parse(parseCtx);
    WC_GUARD(ident, nullptr);
    parseCtx.skipNewlines();    // Skip any newlines that follow
    
    // See if the type for the variable is specified:
    Type * type = nullptr;
    
    if (parseCtx.tok()->type == TokenType::kColon) {
        parseCtx.nextTok();         // Type specified, skip the ':'
        parseCtx.skipNewlines();    // Skip any newlines that follow
        
        // Parse the type:
        type = Type::parse(parseCtx);
        WC_GUARD(type, nullptr);
        parseCtx.skipNewlines();    // Skip any newlines that follow
    }
    
    // Parse the '='
    if (parseCtx.tok()->type != TokenType::kAssign) {
        parseCtx.error("Expected '=' following variable name for variable declaration!");
        return nullptr;
    }
    
    parseCtx.nextTok();         // Consume '='
    parseCtx.skipNewlines();    // Skip any newlines that follow
    
    // Parse the init expression and return result of parsing
    AssignExpr * initExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(initExpr, nullptr);
    
    // Now return the AST node:
    if (type) {
        return WC_NEW_AST_NODE(parseCtx, VarDeclExplicitType, *varToken, *ident, *type, *initExpr);
    }
    else {
        return WC_NEW_AST_NODE(parseCtx, VarDeclInferType, *varToken, *ident, *initExpr);
    }
}

VarDecl::VarDecl(const Token & token,
                 Identifier & ident,
                 AssignExpr & initExpr)
:
    mStartToken(token),
    mIdent(ident),
    mInitExpr(initExpr)
{
    mIdent.mParent = this;
    mInitExpr.mParent = this;
}

const Token & VarDecl::getStartToken() const {
    return mStartToken;
}

const Token & VarDecl::getEndToken() const {
    return mInitExpr.getEndToken();
}

bool VarDecl::allCodepathsHaveUncondRet() const {
    return false;
}

//-----------------------------------------------------------------------------
// VarDeclInferType
//-----------------------------------------------------------------------------
VarDeclInferType::VarDeclInferType(const Token & startToken,
                                   Identifier & ident,
                                   AssignExpr & initExpr)
:
    VarDecl(startToken, ident, initExpr)
{
    WC_EMPTY_FUNC_BODY();
}

void VarDeclInferType::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// VarDeclExplicitType
//-----------------------------------------------------------------------------
VarDeclExplicitType::VarDeclExplicitType(const Token & startToken,
                                         Identifier & ident,
                                         Type & type,
                                         AssignExpr & initExpr)
:
    VarDecl(startToken, ident, initExpr),
    mType(type)
{
    mType.mParent = this;
}

void VarDeclExplicitType::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
