#include "WCFuncCallArgList.hpp"

#include "WCAssignExpr.hpp"
#include "WCLinearAlloc.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// FuncCallArgList
//-----------------------------------------------------------------------------

bool FuncCallArgList::peek(const Token * tokenPtr) {
    return AssignExpr::peek(tokenPtr);
}
    
FuncCallArgList * FuncCallArgList::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the first arg expression
    AssignExpr * expr = AssignExpr::parse(tokenPtr, alloc);
    WC_GUARD(expr, nullptr);
    
    // See if a comma follows (more args)
    if (tokenPtr->type == TokenType::kComma) {
        // Arg list with 2 or more args, consume the ',':
        ++tokenPtr;
        
        // There should be an argument list following:
        FuncCallArgList * argList = FuncCallArgList::parse(tokenPtr, alloc);
        WC_GUARD(argList, nullptr);
        
        // Return the node!
        return WC_NEW_AST_NODE(alloc, FuncCallArgListMulti, *expr, *argList);
    }
    
    // Arg list with just 1 arg:
    return WC_NEW_AST_NODE(alloc, FuncCallArgListSingle, *expr);
}

//-----------------------------------------------------------------------------
// FuncCallArgListSingle
//-----------------------------------------------------------------------------

FuncCallArgListSingle::FuncCallArgListSingle(AssignExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & FuncCallArgListSingle::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & FuncCallArgListSingle::getEndToken() const {
    return mExpr.getEndToken();
}

size_t FuncCallArgListSingle::numArgs() const {
    return 1;
}

void FuncCallArgListSingle::getArgs(std::vector<AssignExpr*> & args) const {
    args.push_back(&mExpr);
}

//-----------------------------------------------------------------------------
// FuncCallArgListMulti
//-----------------------------------------------------------------------------

FuncCallArgListMulti::FuncCallArgListMulti(AssignExpr & expr, FuncCallArgList & argList) :
    mExpr(expr),
    mArgList(argList)
{
    mExpr.mParent = this;
    mArgList.mParent = this;
}

const Token & FuncCallArgListMulti::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & FuncCallArgListMulti::getEndToken() const {
    return mArgList.getEndToken();
}

size_t FuncCallArgListMulti::numArgs() const {
    return 1 + mArgList.numArgs();
}

void FuncCallArgListMulti::getArgs(std::vector<AssignExpr*> & args) const {
    args.push_back(&mExpr);
    mArgList.getArgs(args);
}

WC_END_NAMESPACE
