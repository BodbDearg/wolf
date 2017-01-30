#pragma once

#include "CodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenLNotUnaryOp : public CodegenUnaryOp {
public:
    CodegenLNotUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr);
    
    void visit(const BoolDataType & dataType) override;
};

class CodegenBNotUnaryOp : public CodegenUnaryOp {
public:
    CodegenBNotUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
