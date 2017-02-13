#pragma once

#include "CodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenLNotUnaryOp final : public CodegenUnaryOp {
public:
    CodegenLNotUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr,
                       bool storeResultInExpr);
    
    void visit(const BoolDataType & dataType) override;
};

class CodegenBNotUnaryOp final : public CodegenUnaryOp {
public:
    CodegenBNotUnaryOp(Codegen & cg,
                       const AST::ASTNode & expr,
                       bool storeResultInExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
