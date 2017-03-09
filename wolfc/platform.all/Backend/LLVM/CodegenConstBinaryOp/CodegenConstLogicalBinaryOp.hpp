//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "CodegenConstBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* Base class for classes which codegen compile time binary logical operations. */
class CodegenConstLogicalBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstLogicalBinaryOp(ConstCodegen & cg,
                                const AST::ASTNode & leftExpr,
                                const AST::ASTNode & rightExpr,
                                const char * opSymbol,
                                const char * opName);

    virtual void codegen() override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
