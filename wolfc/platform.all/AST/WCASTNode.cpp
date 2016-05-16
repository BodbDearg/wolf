#include "WCASTNode.hpp"
#include "WCToken.hpp"
#include "WCScope.hpp"
#include <cstdio>
#include <cstdarg>

WC_BEGIN_NAMESPACE

void ASTNode::error(const char * msg, ...) {
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}

void ASTNode::error(const Token & srcToken, const char * msg, ...) {
    // Generic error info
    error("Compile error! Error at: line %zu, column %zu",
          srcToken.srcLine + 1,
          srcToken.srcCol + 1);
    
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

WC_END_NAMESPACE
