#pragma once

#include "CodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenPlusUnaryOp : public CodegenUnaryOp {
public:
    CodegenPlusUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenMinusUnaryOp : public CodegenUnaryOp {
public:
    CodegenMinusUnaryOp(Codegen & cg,
                        const AST::ASTNode & expr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
