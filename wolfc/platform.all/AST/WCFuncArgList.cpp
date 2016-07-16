#include "WCFuncArgList.hpp"
#include "WCFuncArg.hpp"
#include "WCLinearAlloc.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// FuncArgList
//-----------------------------------------------------------------------------

bool FuncArgList::peek(const Token * tokenPtr) {
    return FuncArg::peek(tokenPtr);
}
    
FuncArgList * FuncArgList::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse the first arg
    FuncArg * arg = FuncArg::parse(tokenPtr, alloc);
    WC_GUARD(arg, nullptr);
    
    // See if a comma follows (more args)
    if (tokenPtr->type == TokenType::kComma) {
        // Arg list with 2 or more args, consume the ',':
        ++tokenPtr;
        
        // There should be an argument list following:
        FuncArgList * argList = FuncArgList::parse(tokenPtr, alloc);
        WC_GUARD(argList, nullptr);
        
        // Return the node!
        return WC_NEW_AST_NODE(alloc, FuncArgListMulti, *arg, *argList);
    }
    
    // Arg list with just 1 arg:
    return WC_NEW_AST_NODE(alloc, FuncArgListSingle, *arg);
}

//-----------------------------------------------------------------------------
// FuncArgListSingle
//-----------------------------------------------------------------------------

FuncArgListSingle::FuncArgListSingle(FuncArg & arg) : mArg(arg) {
    mArg.mParent = this;
}

const Token & FuncArgListSingle::getStartToken() const {
    return mArg.getStartToken();
}

const Token & FuncArgListSingle::getEndToken() const {
    return mArg.getEndToken();
}

size_t FuncArgListSingle::numArgs() const {
    return 1;
}

void FuncArgListSingle::getArgs(std::vector<FuncArg*> & args) {
    args.push_back(&mArg);
}

//-----------------------------------------------------------------------------
// FuncArgListMulti
//-----------------------------------------------------------------------------

FuncArgListMulti::FuncArgListMulti(FuncArg & arg, FuncArgList & argList) :
    mArg(arg),
    mArgList(argList)
{
    mArg.mParent = this;
    mArgList.mParent = this;
}

const Token & FuncArgListMulti::getStartToken() const {
    return mArg.getStartToken();
}

const Token & FuncArgListMulti::getEndToken() const {
    return mArgList.getEndToken();
}

size_t FuncArgListMulti::numArgs() const {
    return 1 + mArgList.numArgs();
}

void FuncArgListMulti::getArgs(std::vector<FuncArg*> & args) {
    args.push_back(&mArg);
    mArgList.getArgs(args);
}

WC_END_NAMESPACE