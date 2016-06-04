#include "WCASTNode.hpp"
#include "WCToken.hpp"
#include "WCScope.hpp"
#include <cstdio>
#include <cstdarg>

WC_BEGIN_NAMESPACE

void ASTNode::parseError(const Token & srcToken, const char * msg, ...) {
    // Generic error info
    std::fprintf(stderr,
                 "Error! Parsing failed at line %zu, column %zu! Message:\n",
                 srcToken.startLine + 1,
                 srcToken.startCol + 1);
    
    // Specific error message
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}

ASTNode::ASTNode() : mParent(nullptr) {
    WC_EMPTY_FUNC_BODY();
}

ASTNode::~ASTNode() {
    WC_EMPTY_FUNC_BODY();
}

Scope * ASTNode::getParentScope() {
    Scope * parentScope = dynamic_cast<Scope*>(mParent);
    
    if (parentScope) {
        return parentScope;
    }
    
    if (mParent) {
        return mParent->getParentScope();
    }
    
    return nullptr;
}

void ASTNode::compileError(const char * msg, ...) const {
    // Get the start and end tokens:
    const Token & startToken = getStartToken();
    const Token & endToken = getEndToken();
    
    // Generic error info
    std::fprintf(stderr,
                 "Error! Compile failed for the code beginning at line %zu, column %zu and "
                 "ending at line %zu and column %zu! Message:\n",
                 startToken.startLine + 1,
                 startToken.startCol + 1,
                 endToken.endLine + 1,
                 endToken.endCol + 1);
    
    // Specific error info
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}

WC_END_NAMESPACE
