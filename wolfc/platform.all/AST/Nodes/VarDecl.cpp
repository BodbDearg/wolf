//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

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
    return tokenPtr->type == TokenType::kLet;
}

VarDecl * VarDecl::parse(ParseCtx & parseCtx) {
    // Parse 'let' keyword
    if (parseCtx.tok()->type != TokenType::kLet) {
        parseCtx.error("Expected keyword 'let' at start of a variable declaration!");
        return nullptr;
    }
    
    // Save and skip 'let' and consume any newlines that follow
    const Token * varToken = parseCtx.tok();
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the identifier ahead and skip any newlines that follow
    Identifier * ident = Identifier::parse(parseCtx);
    WC_GUARD(ident, nullptr);
    parseCtx.skipNewlines();
    
    // See if the type for the variable is specified
    Type * type = nullptr;
    
    if (parseCtx.tok()->type == TokenType::kColon) {
        // Type specified, skip the ':' and any newlines that follow
        parseCtx.nextTok();
        parseCtx.skipNewlines();
        
        // Parse the type and skip any newlines that follow:
        type = Type::parse(parseCtx);
        WC_GUARD(type, nullptr);
        parseCtx.skipNewlines();
    }
    
    // Parse the '=' and skip any newlines that follow
    if (parseCtx.tok()->type != TokenType::kAssign) {
        parseCtx.error("Expected '=' following variable name for variable declaration!");
        return nullptr;
    }
    
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the init expression and return result of parsing
    AssignExpr * initExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(initExpr, nullptr);
    
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
