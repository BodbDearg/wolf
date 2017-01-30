#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenAddBinaryOp : public CodegenBinaryOp {
public:
    CodegenAddBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenSubBinaryOp : public CodegenBinaryOp {
public:
    CodegenSubBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenBOrBinaryOp : public CodegenBinaryOp {
public:
    CodegenBOrBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenBXOrBinaryOp : public CodegenBinaryOp {
public:
    CodegenBXOrBinaryOp(Codegen & cg,
                        const AST::ASTNode & leftExpr,
                        const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
