#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenLShiftBinaryOp : public CodegenBinaryOp {
public:
    CodegenLShiftBinaryOp(Codegen & cg,
                          const AST::ASTNode & leftExpr,
                          const AST::ASTNode & rightExpr,
                          bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenARShiftBinaryOp : public CodegenBinaryOp {
public:
    CodegenARShiftBinaryOp(Codegen & cg,
                           const AST::ASTNode & leftExpr,
                           const AST::ASTNode & rightExpr,
                           bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenLRShiftBinaryOp : public CodegenBinaryOp {
public:
    CodegenLRShiftBinaryOp(Codegen & cg,
                           const AST::ASTNode & leftExpr,
                           const AST::ASTNode & rightExpr,
                           bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
