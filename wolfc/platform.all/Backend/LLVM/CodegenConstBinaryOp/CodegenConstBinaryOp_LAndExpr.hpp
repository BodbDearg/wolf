#pragma once

#include "CodegenConstBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenConstLAndBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstLAndBinaryOp(ConstCodegen & cg,
                             const AST::ASTNode & leftExpr,
                             const AST::ASTNode & rightExpr);
    
    void visit(const BoolDataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
