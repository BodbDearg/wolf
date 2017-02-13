#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenLOrBinaryOp final : public CodegenBinaryOp {
public:
    CodegenLOrBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    void visit(const BoolDataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
