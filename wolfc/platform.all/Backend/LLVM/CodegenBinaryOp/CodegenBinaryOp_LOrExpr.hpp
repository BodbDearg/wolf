#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenLOrBinaryOp : public CodegenBinaryOp {
public:
    CodegenLOrBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr);
    
    void visit(const BoolDataType & dataType) override;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
