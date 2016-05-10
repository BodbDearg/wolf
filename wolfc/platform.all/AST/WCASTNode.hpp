#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

struct Token;

/**
 * Abstract base class for all AST nodes.
 */
class ASTNode {
public:
    /* Generates the code for this AST node */
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) = 0;
    
    /* Emit a lexer error to stderror. Line and column information are emitted also. */
    void error(const Token & srcToken, const char * msg, ...);
};

WC_END_NAMESPACE
