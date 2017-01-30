#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenCmpEQBinaryOp : public CodegenBinaryOp {
public:
    CodegenCmpEQBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr);
    
    void visit(const BoolDataType & dataType) override;
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpNEBinaryOp : public CodegenBinaryOp {
public:
    CodegenCmpNEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr);
    
    void visit(const BoolDataType & dataType) override;
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpLTBinaryOp : public CodegenBinaryOp {
public:
    CodegenCmpLTBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpLEBinaryOp : public CodegenBinaryOp {
public:
    CodegenCmpLEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpGTBinaryOp : public CodegenBinaryOp {
public:
    CodegenCmpGTBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpGEBinaryOp : public CodegenBinaryOp {
public:
    CodegenCmpGEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
