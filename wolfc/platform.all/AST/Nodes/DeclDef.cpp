#include "DeclDef.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Func.hpp"
#include "LinearAlloc.hpp"
#include "VarDecl.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// DeclDef
//-----------------------------------------------------------------------------
bool DeclDef::peek(const Token * tokenPtr) {
    return Func::peek(tokenPtr) || VarDecl::peek(tokenPtr);
}

DeclDef * DeclDef::parse(ParseCtx & parseCtx) {
    #warning Handle newlines during parsing
    // Check for 'Func':
    if (Func::peek(parseCtx.tok())) {
        Func * func = Func::parse(parseCtx);
        WC_GUARD(func, nullptr);
        return WC_NEW_AST_NODE(parseCtx, DeclDefFunc, *func);
    }
    
    // Check for 'VarDecl':
    if (VarDecl::peek(parseCtx.tok())) {
        VarDecl * varDecl = VarDecl::parse(parseCtx);
        WC_GUARD(varDecl, nullptr);
        return WC_NEW_AST_NODE(parseCtx, DeclDefVarDecl, *varDecl);
    }
    
    // Unknown stuff ahead
    parseCtx.error("Expected declaration or definition!");
    return nullptr;
}

//-----------------------------------------------------------------------------
// DeclDefFunc
//-----------------------------------------------------------------------------
DeclDefFunc::DeclDefFunc(Func & func) : mFunc(func) {
    mFunc.mParent = this;
}

void DeclDefFunc::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & DeclDefFunc::getStartToken() const {
    return mFunc.getStartToken();
}

const Token & DeclDefFunc::getEndToken() const {
    return mFunc.getEndToken();
}

//-----------------------------------------------------------------------------
// DeclDefVarDecl
//-----------------------------------------------------------------------------
DeclDefVarDecl::DeclDefVarDecl(VarDecl & varDecl) : mVarDecl(varDecl) {
    mVarDecl.mParent = this;
}

void DeclDefVarDecl::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & DeclDefVarDecl::getStartToken() const {
    return mVarDecl.getStartToken();
}
    
const Token & DeclDefVarDecl::getEndToken() const {
    return mVarDecl.getEndToken();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
