#pragma once

#include "CodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenPlusUnaryOp final : public CodegenUnaryOp {
public:
    CodegenPlusUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr,
                       bool storeResultInExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenMinusUnaryOp final : public CodegenUnaryOp {
public:
    CodegenMinusUnaryOp(Codegen & cg,
                        const AST::ASTNode & expr,
                        bool storeResultInExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
