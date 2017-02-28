//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Func.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "FuncArg.hpp"
#include "Identifier.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"
#include "Type.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <set>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool Func::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kFunc;
}

Func * Func::parse(ParseCtx & parseCtx) {
    // Must be a function ahead:
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected function!");
        return nullptr;
    }
    
    // Skip the 'func' token and save location. Also skip any newlines that follow:
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    parseCtx.skipNewlines();
    
    // Parse the function identifier and skip any newlines that follow:
    Identifier * identifier = Identifier::parse(parseCtx);
    WC_GUARD(identifier, nullptr);
    parseCtx.skipNewlines();
    
    // Expect '('
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("'(' expected following function name!");
        return nullptr;
    }
    
    parseCtx.nextTok();         // Skip '('
    parseCtx.skipNewlines();    // Skip any newlines that follow
    
    // Parse any function arguments that follow:
    std::vector<FuncArg*> funcArgs;
    
    while (FuncArg::peek(parseCtx.tok())) {
        // Parse the arg
        FuncArg * funcArg = FuncArg::parse(parseCtx);
        
        if (funcArg) {
            funcArgs.push_back(funcArg);
        }
        
        // Skip any newlines that follow
        parseCtx.skipNewlines();
        
        // See if a ',' follows
        if (parseCtx.tok()->type == TokenType::kComma) {
            // Skip the ',' and any newlines that follow
            parseCtx.nextTok();
            parseCtx.skipNewlines();
            
            // Expect an argument following
            if (!FuncArg::peek(parseCtx.tok())) {
                parseCtx.error("Expect an argument following ','!");
                parseCtx.skipNewlines();
            }
        }
    }
    
    // Expect ')'
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("')' expected to close args list of function!");
        return nullptr;
    }
    
    // Skip ')'
    parseCtx.nextTok();
    
    // Skip any newlines that follow. Tentatively mark also whether the function body
    // is on a new line or not compared to the function opener (note: we may find this
    // not to be true below)
    bool funcBodyIsOnNewLine = parseCtx.skipNewlines() > 0;
    
    // See if a '->' follows for function explicit return type.
    // If it is not present then a 'void' return type is assumed.
    Type * returnType = nullptr;
    
    if (parseCtx.tok()->type == TokenType::kOpArrow) {
        // Explicit return type, skip the '->' first.
        // Also don't regard the the function body as being on a new line for now.
        parseCtx.nextTok();
        funcBodyIsOnNewLine = false;
        
        // Skip any newlines that follow
        parseCtx.skipNewlines();
        
        // Now parse the return type, if that fails then bail
        returnType = Type::parse(parseCtx);
        WC_GUARD(returnType, nullptr);
    }
    
    // Skip any newlines that follow
    if (parseCtx.skipNewlines() > 0) {
        funcBodyIsOnNewLine = true;
    }
    
    // If a 'do' token follows the function opener then the body is allowed to be on
    // the same line as the function signature:
    bool funcBodyMustBeOnNewLine = true;
    
    if (parseCtx.tok()->type == TokenType::kDo) {
        // The function body doesn't have to be on a new line, have a 'do' token.
        // Skip this token and lift the newline restriction:
        parseCtx.nextTok();
        funcBodyMustBeOnNewLine = false;
        
        // Skip any newlines that follow
        if (parseCtx.skipNewlines() > 0) {
            funcBodyIsOnNewLine = true;
        }
    }
    
    // If the function body is required to be on a new line, make sure that is the case here:
    if (funcBodyMustBeOnNewLine && !funcBodyIsOnNewLine) {
        parseCtx.error("Code following a function signature (name, params & return type) must "
                       "be on a new line unless 'do' is used after the function signature.");
    }
    
    // Parse the inner function scope:
    Scope * scope = Scope::parse(parseCtx);
    WC_GUARD(scope, nullptr);
    
    // Skip any newlines that follow
    parseCtx.skipNewlines();
    
    // Must be terminated by an 'end' token
    if (parseCtx.tok()->type != TokenType::kEnd) {
        parseCtx.error("'end' expected to terminate function definition!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Done: return the parsed function
    return WC_NEW_AST_NODE(parseCtx,
                           Func,
                           *startToken,
                           *identifier,
                           std::move(funcArgs),
                           returnType,
                           *scope,
                           *endToken);
}

Func::Func(const Token & startToken,
           Identifier & identifier,
           std::vector<FuncArg*> && funcArgs,
           Type * explicitReturnType,
           Scope & scope,
           const Token & endToken)
:
    mStartToken(startToken),
    mIdentifier(identifier),
    mFuncArgs(funcArgs),
    mExplicitReturnType(explicitReturnType),
    mScope(scope),
    mEndToken(endToken)
{
    mIdentifier.mParent = this;
    
    for (FuncArg * funcArg : mFuncArgs) {
        funcArg->mParent = this;
    }
    
    if (mExplicitReturnType) {
        mExplicitReturnType->mParent = this;
    }
    
    mScope.mParent = this;
}

void Func::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & Func::getStartToken() const {
    return mStartToken;
}

const Token & Func::getEndToken() const {
    return mEndToken;
}

const char * Func::name() const {
    return mIdentifier.name();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
