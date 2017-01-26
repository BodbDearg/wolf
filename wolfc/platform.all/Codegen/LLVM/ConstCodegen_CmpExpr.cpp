#include "ConstCodegen.hpp"

#include "AST/Nodes/WCAddExpr.hpp"
#include "AST/Nodes/WCCmpExpr.hpp"
#include "CodegenConstBinaryOpHelper.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstCmpBinaryOp : public CodegenConstBinaryOpHelper {
public:
    CodegenConstCmpBinaryOp(ConstCodegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr,
                            const char * opSymbol,
                            const char * opName)
    :
        CodegenConstBinaryOpHelper(cg, leftExpr, rightExpr, opSymbol, opName)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    inline void codegenIntCompare(llvm::ICmpInst::Predicate predicate) {
        pushOpResult(llvm::ConstantExpr::getICmp(predicate,
                                                 mLeftVal,
                                                 mRightVal));
    }
};

class CodegenConstCmpEQBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpEQBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "==", "equals")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_EQ);
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_EQ);
    }
};

class CodegenConstCmpNEBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpNEBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "!=", "not equals")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_NE);
    }
    
    void visit(const BoolDataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_NE);
    }
};

class CodegenConstCmpLTBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpLTBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "<", "less than")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SLT);
    }
};

class CodegenConstCmpLEBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpLEBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "<=", "less than or equal to")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SLE);
    }
};

class CodegenConstCmpGTBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpGTBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, ">", "greater than")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SGT);
    }
};

class CodegenConstCmpGEBinaryOp : public CodegenConstCmpBinaryOp {
public:
    CodegenConstCmpGEBinaryOp(ConstCodegen & cg, const AST::ASTNode & leftExpr, const AST::ASTNode & rightExpr) :
        CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, ">=", "greater than or equal to")
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    void visit(const Int64DataType & dataType) override {
        WC_UNUSED_PARAM(dataType);
        codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SGE);
    }
};

void ConstCodegen::visit(const AST::CmpExprNoOp & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void ConstCodegen::visit(const AST::CmpExprEQ & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpEQBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprNE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpNEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprLT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpLTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprLE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpLEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprGT & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpGTBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

void ConstCodegen::visit(const AST::CmpExprGE & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    CodegenConstCmpGEBinaryOp(*this, astNode.mLeftExpr, astNode.mRightExpr).codegen();
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
