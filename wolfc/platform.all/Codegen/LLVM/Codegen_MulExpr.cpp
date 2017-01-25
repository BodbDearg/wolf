#include "Codegen.hpp"

#include "AST/Nodes/WCMulExpr.hpp"
#include "AST/Nodes/WCShiftExpr.hpp"
#include "CodegenBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenMulBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenMulBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "*", "multiply")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateMul(mLeftVal, mRightVal, "Int64:Mul:Result"));
    }
};

class CodegenDivBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenDivBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "/", "divide")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateSDiv(mLeftVal, mRightVal, "Int64:Div:Result"));
    }
};

class CodegenModBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenModBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "%", "modulus")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateSRem(mLeftVal, mRightVal, "Int64:Mod:Result"));
    }
};

class CodegenBAndBinaryOp : public CodegenBinaryOpHelper {
public:
    CodegenBAndBinaryOp(Codegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenBinaryOpHelper(cg, leftExpr, rightExpr, "&", "bitwise and")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        pushOpResult(mCG.mCtx.mIRBuilder.CreateAnd(mLeftVal, mRightVal, "Int64:BAnd:Result"));
    }
};

void Codegen::visit(const AST::MulExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::MulExprMul & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenMulBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::MulExprDiv & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenDivBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::MulExprMod & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenModBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void Codegen::visit(const AST::MulExprBAnd & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenBAndBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
