#pragma once

#include "CodegenConstBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenConstMulBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstMulBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstDivBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstDivBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstModBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstModBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstBAndBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstBAndBinaryOp(ConstCodegen & cg,
                             const AST::ASTNode & leftExpr,
                             const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
