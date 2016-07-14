#include "WCDeclDef.hpp"
#include "WCFunc.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// DeclDef
//-----------------------------------------------------------------------------

bool DeclDef::peek(const Token * tokenPtr) {
    return Func::peek(tokenPtr);
}

DeclDef * DeclDef::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // See if function follows:
    if (Func::peek(tokenPtr)) {
        Func * func = Func::parse(tokenPtr, alloc);
        WC_GUARD(func, nullptr);
        return WC_NEW_AST_NODE(alloc, DeclDefFunc, *func);
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

WC_END_NAMESPACE
