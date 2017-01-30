#pragma once

#include "CodegenConstBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstLShiftBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstLShiftBinaryOp(ConstCodegen & cg, 
                               const AST::ASTNode & leftExpr,
                               const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstARShiftBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstARShiftBinaryOp(ConstCodegen & cg,
                                const AST::ASTNode & leftExpr,
                                const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstLRShiftBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstLRShiftBinaryOp(ConstCodegen & cg,
                                const AST::ASTNode & leftExpr,
                                const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
