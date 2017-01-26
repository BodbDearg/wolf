#include "ConstCodegen.hpp"

#include "AST/Nodes/WCCmpExpr.hpp"
#include "AST/Nodes/WCNotExpr.hpp"
#include "CodegenConstUnaryOpHelper.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstLNotUnaryOp : public CodegenConstUnaryOpHelper {
public:
    CodegenConstLNotUnaryOp(ConstCodegen & cg, const AST::ASTNode & expr) :
        CodegenConstUnaryOpHelper(cg, expr, "not", "logical not")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getNot(mExprVal));
    }
};

class CodegenConstBNotUnaryOp : public CodegenConstUnaryOpHelper {
public:
    CodegenConstBNotUnaryOp(ConstCodegen & cg, const AST::ASTNode & expr) :
        CodegenConstUnaryOpHelper(cg, expr, "~", "bitwise not")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(llvm::ConstantExpr::getNot(mExprVal));
    }
};

void ConstCodegen::visit(const AST::NotExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::NotExprLNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstLNotUnaryOp(*this, astNode.mExpr).codegen();
}

void ConstCodegen::visit(const AST::NotExprBNot & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstBNotUnaryOp(*this, astNode.mExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
