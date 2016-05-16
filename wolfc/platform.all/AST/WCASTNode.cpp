#include "WCASTNode.hpp"
#include "WCToken.hpp"
#include <cstdio>
#include <cstdarg>

WC_BEGIN_NAMESPACE

ASTNode::ASTNode(ASTNode * parent) : mParent(parent) {
    WC_EMPTY_FUNC_BODY();
}

ASTNode::~ASTNode() {
    WC_EMPTY_FUNC_BODY();
}

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

WC_END_NAMESPACE
