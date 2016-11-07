#include "WCDeclDefs.hpp"

#include "WCDeclDef.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// DeclDefs
//-----------------------------------------------------------------------------

bool DeclDefs::peek(const Token * tokenPtr) {
    return DeclDef::peek(tokenPtr);
}

DeclDefs * DeclDefs::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Expect there to be something:
    if (!peek(tokenPtr)) {
        parseError(*tokenPtr, "Expected declarations and definitions!");
        return nullptr;
    }
    
    // Parse the first decl def:
    DeclDef * declDef = DeclDef::parse(tokenPtr, alloc);
    WC_GUARD(declDef, nullptr);
    
    // See if more follow:
    if (DeclDefs::peek(tokenPtr)) {
        // Multi decl defs:
        DeclDefs * declDefs = DeclDefs::parse(tokenPtr, alloc);
        WC_GUARD(declDefs, nullptr);
        return WC_NEW_AST_NODE(alloc, DeclDefsMulti, *declDef, *declDefs);
    }
    
    // Single decl def:
    return WC_NEW_AST_NODE(alloc, DeclDefsSingle, *declDef);
}

//-----------------------------------------------------------------------------
// DeclDefsSingle
//-----------------------------------------------------------------------------

DeclDefsSingle::DeclDefsSingle(DeclDef & declDef) : mDeclDef(declDef) {
    mDeclDef.mParent = this;
}

const Token & DeclDefsSingle::getStartToken() const {
    return mDeclDef.getStartToken();
}

const Token & DeclDefsSingle::getEndToken() const {
    return mDeclDef.getEndToken();
}

bool DeclDefsSingle::codegen(CodegenCtx & cgCtx) {
    return mDeclDef.codegen(cgCtx);
}

//-----------------------------------------------------------------------------
// DeclDefsMulti
//-----------------------------------------------------------------------------

DeclDefsMulti::DeclDefsMulti(DeclDef & declDef, DeclDefs & declDefs) :
    mDeclDef(declDef),
    mDeclDefs(declDefs)
{
    mDeclDef.mParent = this;
    mDeclDefs.mParent = this;
}

const Token & DeclDefsMulti::getStartToken() const {
    return mDeclDef.getStartToken();
}

const Token & DeclDefsMulti::getEndToken() const {
    return mDeclDefs.getEndToken();
}

bool DeclDefsMulti::codegen(CodegenCtx & cgCtx) {
    return mDeclDef.codegen(cgCtx) && mDeclDefs.codegen(cgCtx);
}

WC_END_NAMESPACE
