#pragma once

#include "CodegenConstBinaryOp.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstCmpBinaryOp : public CodegenConstBinaryOp {
public:
    CodegenConstCmpBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr,
                            const char * opSymbol,
                            const char * opName);
    
    void codegenIntCompare(llvm::ICmpInst::Predicate predicate);
};

class CodegenConstCmpEQBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpEQBinaryOp(ConstCodegen & cg,
                              const AST::ASTNode & leftExpr,
                              const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
    void visit(const BoolDataType & dataType) override;
};

class CodegenConstCmpNEBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpNEBinaryOp(ConstCodegen & cg,
                              const AST::ASTNode & leftExpr,
                              const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
    void visit(const BoolDataType & dataType) override;
};

class CodegenConstCmpLTBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpLTBinaryOp(ConstCodegen & cg,
                              const AST::ASTNode & leftExpr,
                              const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstCmpLEBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpLEBinaryOp(ConstCodegen & cg,
                              const AST::ASTNode & leftExpr,
                              const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstCmpGTBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpGTBinaryOp(ConstCodegen & cg,
                              const AST::ASTNode & leftExpr,
                              const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenConstCmpGEBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpGEBinaryOp(ConstCodegen & cg,
                              const AST::ASTNode & leftExpr,
                              const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
