#pragma once

#include "CodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenIncUnaryOp : public CodegenUnaryOp {
public:
    CodegenIncUnaryOp(Codegen & cg,
                      const AST::ASTNode & expr,
                      bool storeResultInExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenDecUnaryOp : public CodegenUnaryOp {
public:
    CodegenDecUnaryOp(Codegen & cg,
                      const AST::ASTNode & expr,
                      bool storeResultInExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
