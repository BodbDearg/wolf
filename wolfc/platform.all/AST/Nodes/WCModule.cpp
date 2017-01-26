#include "WCModule.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "WCDeclDef.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

Module * Module::parse(ParseCtx & parseCtx) {
    // Parse a list of decldefs for the module.
    // Try to parse as many as possible so we get multiple error messages for various problems.
    std::vector<DeclDef*> declDefs;
    
    while (parseCtx.tok()->type != TokenType::kEOF) {
        // Save this in case we need it later
        const Token * startTok = parseCtx.tok();
        
        // Try to parse the decldef
        DeclDef * declDef = DeclDef::parse(parseCtx);
        
        if (declDef) {
            // Got something from the parse, save:
            declDefs.push_back(declDef);
        }
        else if (!parseCtx.hasErrors()) {
            // Just in case the code failed to emit an error
            parseCtx.error(*startTok, "Failed to parse a top level module element! Exact error unknown.");
        }
        
        // Skip junk if getting a DeclDef failed:
        while (parseCtx.tok()->type != TokenType::kEOF) {
            if (DeclDef::peek(parseCtx.tok())) {
                break;
            }
            
            parseCtx.nextTok();
        }
    }
    
    // Okay, create and return the module
    return WC_NEW_AST_NODE(parseCtx, Module, std::move(declDefs), *parseCtx.tok());
}

Module::Module(std::vector<DeclDef*> && declDefs, const Token & eofToken) :
    mDeclDefs(declDefs),
    mEOFToken(eofToken)
{
    WC_EMPTY_FUNC_BODY();
}

void Module::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & Module::getStartToken() const {
    if (!mDeclDefs.empty()) {
        return mDeclDefs.front()->getStartToken();
    }
    
    return mEOFToken;
}

const Token & Module::getEndToken() const {
    return mEOFToken;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
