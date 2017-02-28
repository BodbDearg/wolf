//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

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
};

class CodegenCmpEQBinaryOp final : public CodegenCmpBinaryOp {
public:
    CodegenCmpEQBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenCmpNEBinaryOp final : public CodegenCmpBinaryOp {
public:
    CodegenCmpNEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenCmpLTBinaryOp final : public CodegenCmpBinaryOp {
public:
    CodegenCmpLTBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenCmpLEBinaryOp final : public CodegenCmpBinaryOp {
public:
    CodegenCmpLEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenCmpGTBinaryOp final : public CodegenCmpBinaryOp {
public:
    CodegenCmpGTBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenCmpGEBinaryOp final : public CodegenCmpBinaryOp {
public:
    CodegenCmpGEBinaryOp(Codegen & cg,
                         const AST::ASTNode & leftExpr,
                         const AST::ASTNode & rightExpr,
                         bool storeResultOnLeft);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
