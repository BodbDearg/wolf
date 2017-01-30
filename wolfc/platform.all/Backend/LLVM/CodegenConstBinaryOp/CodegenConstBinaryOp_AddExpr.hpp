#pragma once

#include "CodegenConstBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenConstAddBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstAddBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstSubBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstSubBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstBOrBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstBOrBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstBXOrBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstBXOrBinaryOp(ConstCodegen & cg,
                             const AST::ASTNode & leftExpr,
                             const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
