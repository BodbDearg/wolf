#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenMulBinaryOp final : public CodegenBinaryOp {
public:
    CodegenMulBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenDivBinaryOp final : public CodegenBinaryOp {
public:
    CodegenDivBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenModBinaryOp final : public CodegenBinaryOp {
public:
    CodegenModBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenBAndBinaryOp final : public CodegenBinaryOp {
public:
    CodegenBAndBinaryOp(Codegen & cg,
                        const AST::ASTNode & leftExpr,
                        const AST::ASTNode & rightExpr,
                        bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
