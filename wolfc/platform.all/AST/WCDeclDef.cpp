#include "WCDeclDef.hpp"

#include "WCFunc.hpp"
#include "WCLinearAlloc.hpp"
#include "WCVarDecl.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// DeclDef
//-----------------------------------------------------------------------------
bool DeclDef::peek(const Token * tokenPtr) {
    return Func::peek(tokenPtr) || VarDecl::peek(tokenPtr);
}

DeclDef * DeclDef::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Check for 'Func':
    if (Func::peek(tokenPtr)) {
        Func * func = Func::parse(tokenPtr, alloc);
        WC_GUARD(func, nullptr);
        return WC_NEW_AST_NODE(alloc, DeclDefFunc, *func);
    }
    
    // Check for 'VarDecl':
    if (VarDecl::peek(tokenPtr)) {
        VarDecl * varDecl = VarDecl::parse(tokenPtr, alloc);
        WC_GUARD(varDecl, nullptr);
        return WC_NEW_AST_NODE(alloc, DeclDefVarDecl, *varDecl);
    }
    
    // Unknown stuff ahead
    parseError(*tokenPtr, "Expected declaration or definition!");
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

bool DeclDefFunc::codegen(CodegenCtx & cgCtx) {
    return mFunc.codegen(cgCtx);
}

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
    
bool DeclDefVarDecl::codegen(CodegenCtx & cgCtx) {
    return mVarDecl.codegen(cgCtx);
}

WC_END_NAMESPACE
