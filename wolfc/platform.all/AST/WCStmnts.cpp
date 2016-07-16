#include "WCStmnts.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCStmnt.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Stmnts
//-----------------------------------------------------------------------------

bool Stmnts::peek(const Token * tokenPtr) {
    return Stmnt::peek(tokenPtr);
}
    
Stmnts * Stmnts::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    const Token * startTok = tokenPtr;
    Stmnt * stmnt = Stmnt::parse(tokenPtr, alloc);
    WC_GUARD(stmnt, nullptr);
    
    if (Stmnts::peek(tokenPtr)) {
        if (tokenPtr->startLine <= startTok->startLine) {
            // TODO: support semi colon for multiple expressions per line
            parseError(*tokenPtr, "Multiple expressions are not allowed on one line!");
            return nullptr;
        }
        
        Stmnts * stmnts = Stmnts::parse(tokenPtr, alloc);
        WC_GUARD(stmnts, nullptr);
        return WC_NEW_AST_NODE(alloc, StmntsMulti, *stmnt, *stmnts);
    }
    else {
        return WC_NEW_AST_NODE(alloc, StmntsSingle, *stmnt);
    }
}

//-----------------------------------------------------------------------------
// StmntsSingle
//-----------------------------------------------------------------------------

StmntsSingle::StmntsSingle(Stmnt & stmnt) : mStmnt(stmnt) {
    mStmnt.mParent = this;
}

const Token & StmntsSingle::getStartToken() const {
    return mStmnt.getStartToken();
}

const Token & StmntsSingle::getEndToken() const {
    return mStmnt.getEndToken();
}

bool StmntsSingle::codegen(CodegenCtx & cgCtx) {
    return mStmnt.codegen(cgCtx);
};

//-----------------------------------------------------------------------------
// StmntsMulti
//-----------------------------------------------------------------------------

StmntsMulti::StmntsMulti(Stmnt & leftStmnt, Stmnts & rightSmnts) :
    mLeftStmnt(leftStmnt),
    mRightStmnts(rightSmnts)
{
    mLeftStmnt.mParent = this;
    mRightStmnts.mParent = this;
}

const Token & StmntsMulti::getStartToken() const {
    return mLeftStmnt.getStartToken();
}

const Token & StmntsMulti::getEndToken() const {
    return mRightStmnts.getEndToken();
}

bool StmntsMulti::codegen(CodegenCtx & cgCtx) {
    WC_GUARD(mLeftStmnt.codegen(cgCtx), false);
    WC_GUARD(mRightStmnts.codegen(cgCtx), false);
    return true;
}

WC_END_NAMESPACE
