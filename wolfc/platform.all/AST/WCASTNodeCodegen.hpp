#pragma once

#include "WCASTNode.hpp"

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

struct CodegenCtx;

/* Abstract base class for all AST nodes which generate one LLVM code element directly. */
class ASTNodeCodegen : public ASTNode {
public:
    ASTNodeCodegen(ASTNode & parent);
    
    /* Generates the code for this AST node */
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) = 0;
};

WC_END_NAMESPACE
