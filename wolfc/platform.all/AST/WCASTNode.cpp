#include "WCASTNode.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

void ASTNode::error(const Token & srcToken, const char * msg, ...) {
    // Generic error info
    std::fprintf(stderr,
                 "Compile error! Error at: line %zu, column %zu\n",
                 (srcToken.srcLine + 1),
                 (srcToken.srcCol + 1));
    
    // Specific error message
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
}

WC_END_NAMESPACE
