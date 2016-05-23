#include "WCStmnts.hpp"
#include "WCCodegenCtx.hpp"
#include "WCStmnt.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Stmnts
//-----------------------------------------------------------------------------

bool Stmnts::peek(const Token * tokenPtr) {
    return Stmnt::peek(tokenPtr);
}
    
Stmnts * Stmnts::parse(const Token *& tokenPtr) {
    const Token * startTok = tokenPtr;
    Stmnt * stmnt = Stmnt::parse(tokenPtr);
    WC_GUARD(stmnt, nullptr);
    
    if (Stmnts::peek(tokenPtr)) {
        if (tokenPtr->startLine <= startTok->startLine) {
            // TODO: support semi colon for multiple expressions per line
            parseError(*tokenPtr, "Multiple expressions are not allowed on one line!");
            return nullptr;
        }
        
        Stmnts * stmnts = Stmnts::parse(tokenPtr);
        WC_GUARD(stmnts, nullptr);
        return new StmntsMulti(*stmnt, *stmnts);
    }
    else {
        return new StmntsSingle(*stmnt);
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

llvm::Value * StmntsSingle::generateCode(const CodegenCtx & cgCtx) {
    return mStmnt.generateCode(cgCtx);
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

llvm::Value * StmntsMulti::generateCode(const CodegenCtx & cgCtx) {
    // Generate the instructions for all these calls
    WC_GUARD(mLeftStmnt.generateCode(cgCtx), nullptr);
    WC_GUARD(mRightStmnts.generateCode(cgCtx), nullptr);
    
    // Return the last instruction generated
    return &(*(cgCtx.irBuilder.GetInsertPoint()));
}

WC_END_NAMESPACE
