#pragma once

#include "CodegenConstUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstPlusUnaryOp : public CodegenConstUnaryOp {
public:
    CodegenConstPlusUnaryOp(ConstCodegen & cg,
                            const AST::ASTNode & expr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstMinusUnaryOp : public CodegenConstUnaryOp {
public:
    CodegenConstMinusUnaryOp(ConstCodegen & cg,
                             const AST::ASTNode & expr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
