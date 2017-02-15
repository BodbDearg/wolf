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
    #warning Handle newlines during parsing
    return tokenPtr->type == TokenType::kFunc;
}

Func * Func::parse(ParseCtx & parseCtx) {
    // Must be a function ahead:
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected function!");
        return nullptr;
    }
    
    // Skip the 'func' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();         // Consume 'func'
    parseCtx.skipNewlines();    // Skip any newlines
    
    // Parse the function identifier:
    Identifier * identifier = Identifier::parse(parseCtx);
    WC_GUARD(identifier, nullptr);
    parseCtx.skipNewlines();    // Skip any newlines
    
    // Expect '('
    if (parseCtx.tok()->type != TokenType::kLParen) {
        parseCtx.error("'(' expected following function name!");
        return nullptr;
    }
    
    parseCtx.nextTok();         // Skip '('
    parseCtx.skipNewlines();    // Skip any newlines
    
    // Parse any function arguments that follow:
    std::vector<FuncArg*> funcArgs;
    
    while (FuncArg::peek(parseCtx.tok())) {
        // Parse the arg
        FuncArg * funcArg = FuncArg::parse(parseCtx);
        
        if (funcArg) {
            funcArgs.push_back(funcArg);
        }
        
        parseCtx.skipNewlines();    // Skip any newlines
        
        // See if a ',' follows
        if (parseCtx.tok()->type == TokenType::kComma) {
            parseCtx.nextTok();         // Skip the ','
            parseCtx.skipNewlines();    // Skip any newlines
            
            // Expect an argument following
            if (!FuncArg::peek(parseCtx.tok())) {
                parseCtx.error("Expect an argument following ','!");
            }
        }
    }
    
    // Expect ')'
    if (parseCtx.tok()->type != TokenType::kRParen) {
        parseCtx.error("')' expected to close args list of function!");
        return nullptr;
    }
    
    parseCtx.nextTok();         // Skip ')'
    parseCtx.skipNewlines();    // Skip any newlines
    
    // See if a '->' follows for function explicit return type.
    // If it is not present then a 'void' return type is assumed.
    Type * returnType = nullptr;
    
    if (parseCtx.tok()->type == TokenType::kOpArrow) {
        parseCtx.nextTok();         // Explicit return type, skip the '->' first
        parseCtx.skipNewlines();    // Skip any newlines
        
        // Now parse the return type, if that fails then bail
        returnType = Type::parse(parseCtx);
        WC_GUARD(returnType, nullptr);
    }
    
    parseCtx.skipNewlines();    // Skip any newlines
    
    // Parse the inner function scope:
    Scope * scope = Scope::parse(parseCtx);
    WC_GUARD(scope, nullptr);
    
    // Skip any newlines
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
