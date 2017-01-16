#include "WCDeclDef.hpp"

#include "WCFunc.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCVarDecl.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// DeclDef
//-----------------------------------------------------------------------------
bool DeclDef::peek(const Token * tokenPtr) {
    return Func::peek(tokenPtr) || VarDecl::peek(tokenPtr);
}

DeclDef * DeclDef::parse(ParseCtx & parseCtx) {
    // Check for 'Func':
    if (Func::peek(parseCtx.curTok)) {
        Func * func = Func::parse(parseCtx);
        WC_GUARD(func, nullptr);
        return WC_NEW_AST_NODE(parseCtx, DeclDefFunc, *func);
    }
    
    // Check for 'VarDecl':
    if (VarDecl::peek(parseCtx.curTok)) {
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

const Token & DeclDefFunc::getStartToken() const {
    return mFunc.getStartToken();
}

const Token & DeclDefFunc::getEndToken() const {
    return mFunc.getEndToken();
}

#warning FIXME - Codegen
#if 0
bool DeclDefFunc::codegen(CodegenCtx & cgCtx) {
    return mFunc.codegen(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// DeclDefVarDecl
//-----------------------------------------------------------------------------
DeclDefVarDecl::DeclDefVarDecl(VarDecl & varDecl) : mVarDecl(varDecl) {
    mVarDecl.mParent = this;
}

const Token & DeclDefVarDecl::getStartToken() const {
    return mVarDecl.getStartToken();
}
    
const Token & DeclDefVarDecl::getEndToken() const {
    return mVarDecl.getEndToken();
}

#warning FIXME - Codegen
#if 0
bool DeclDefVarDecl::codegen(CodegenCtx & cgCtx) {
    return mVarDecl.codegen(cgCtx);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
