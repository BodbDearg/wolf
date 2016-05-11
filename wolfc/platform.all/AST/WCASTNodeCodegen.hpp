#pragma once

#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

/**
 * Abstract base class for all AST nodes which generate one LLVM code element directly.
 */
class ASTNodeCodegen : public ASTNode {
public:
    /* Generates the code for this AST node */
    virtual llvm::Value * generateCode(llvm::IRBuilder<> & irBuilder) = 0;
};

WC_END_NAMESPACE
