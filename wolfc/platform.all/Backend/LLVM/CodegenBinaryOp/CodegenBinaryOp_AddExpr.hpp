#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenAddBinaryOp final : public CodegenBinaryOp {
public:
    CodegenAddBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenSubBinaryOp final : public CodegenBinaryOp {
public:
    CodegenSubBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenBOrBinaryOp final : public CodegenBinaryOp {
public:
    CodegenBOrBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenBXOrBinaryOp final : public CodegenBinaryOp {
public:
    CodegenBXOrBinaryOp(Codegen & cg,
                        const AST::ASTNode & leftExpr,
                        const AST::ASTNode & rightExpr,
                        bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
