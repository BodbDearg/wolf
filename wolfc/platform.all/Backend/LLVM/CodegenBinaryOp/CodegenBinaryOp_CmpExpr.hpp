#include "CodegenBinaryOp.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/IRBuilder.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenCmpBinaryOp : public CodegenBinaryOp {
public:
    CodegenCmpBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       const char * opSymbol,
                       const char * opName,
                       bool storeResultOnLeft);
    
protected:
    void codegenICmp(llvm::CmpInst::Predicate predicate,
                     const char * instructionLabel);
};

class CodegenCmpEQBinaryOp : public CodegenCmpBinaryOp {
public:
    CodegenCmpEQBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    void visit(const BoolDataType & dataType) override;
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpNEBinaryOp : public CodegenCmpBinaryOp {
public:
    CodegenCmpNEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    void visit(const BoolDataType & dataType) override;
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpLTBinaryOp : public CodegenCmpBinaryOp {
public:
    CodegenCmpLTBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpLEBinaryOp : public CodegenCmpBinaryOp {
public:
    CodegenCmpLEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpGTBinaryOp : public CodegenCmpBinaryOp {
public:
    CodegenCmpGTBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

class CodegenCmpGEBinaryOp : public CodegenCmpBinaryOp {
public:
    CodegenCmpGEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
