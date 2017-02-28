//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Module.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DeclDef.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

Module * Module::parse(ParseCtx & parseCtx) {
    // Skip any newlines and commas (decldef separators) that follow:
    parseCtx.skipNewlinesAndCommas();
    
    // Parse a list of decldefs for the module.
    // Try to parse as many as possible so we get multiple error messages for various problems.
    std::vector<DeclDef*> declDefs;
    bool requireNewlineBetweenDeclDefs = false;
    
    while (parseCtx.tok()->type != TokenType::kEOF) {
        // Save this in case we need it later
        const Token * startTok = parseCtx.tok();
        
        // Try to parse the decldef
        DeclDef * declDef = DeclDef::parse(parseCtx);
        
        if (declDef) {
            // Each subsequent decldef must be put on a new line, if we find
            // that not to be the case then issue errors:
            //
            // TODO: Allow commas to enable multiple decldefs per line
            if (requireNewlineBetweenDeclDefs) {
                DeclDef * lastDeclDef = declDefs.back();
                const Token & thisDeclDefStart = declDef->getStartToken();
                const Token & prevDeclDefEnd = lastDeclDef->getEndToken();
                
                if (thisDeclDefStart.startLine <= prevDeclDefEnd.startLine) {
                    parseCtx.error("Top level declarations/definitions must be separated onto different lines, or by ','! "
                                   "The declaration/definition starting at line %zu, col %zu is on the same line as "
                                   "the declaration/definition ending at line %zu, col %zu!",
                                   thisDeclDefStart.startLine + 1,
                                   thisDeclDefStart.startCol + 1,
                                   prevDeclDefEnd.startLine + 1,
                                   prevDeclDefEnd.startCol + 1);
                }
            }
            
            // Save the decldef which was parsed
            declDefs.push_back(declDef);
            
            // Skip any newlines and commas that folllow. If any newlines or commas are found, these reset the
            // requirement for the next decldef to be on a new line.
            if (parseCtx.skipNewlinesAndCommas() > 0) {
                requireNewlineBetweenDeclDefs = false;
            }
            else {
                requireNewlineBetweenDeclDefs = true;
            }
        }
        else {
            // Just in case the code failed to emit an error
            if (!parseCtx.hasErrors()) {
                parseCtx.error(*startTok, "Failed to parse a top level module element! Exact error unknown.");
            }
            
            // Skip junk when getting a DeclDef failed:
            while (parseCtx.tok()->type != TokenType::kEOF) {
                if (DeclDef::peek(parseCtx.tok())) {
                    break;
                }
                
                // Skip both the token and any newlines and commas that follow it
                parseCtx.nextTok();
                
                if (parseCtx.skipNewlinesAndCommas() > 0) {
                    requireNewlineBetweenDeclDefs = false;
                }
            }
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
