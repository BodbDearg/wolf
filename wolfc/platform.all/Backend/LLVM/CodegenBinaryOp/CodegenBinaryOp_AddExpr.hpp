//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenBinaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenAddOrSubBinaryOp : public CodegenBinaryOp {
public:
    CodegenAddOrSubBinaryOp(Codegen & cg,
                            const AST::ASTNode & leftExpr,
                            const AST::ASTNode & rightExpr,
                            const char * opSymbol,
                            const char * opName,
                            bool storeResultOnLeft);
    
protected:
    virtual bool verifyLeftAndRightTypesAreOkForOp() final override;
};

class CodegenAddBinaryOp final : public CodegenAddOrSubBinaryOp {
public:
    CodegenAddBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const PtrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenSubBinaryOp final : public CodegenAddOrSubBinaryOp {
public:
    CodegenSubBinaryOp(Codegen & cg,
                       const AST::ASTNode & leftExpr,
                       const AST::ASTNode & rightExpr,
                       bool storeResultOnLeft);
    
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const PtrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
};

class CodegenBOrBinaryOp final : public CodegenBinaryOp {
public:
    CodegenBOrBinaryOp(Codegen & cg,
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

class CodegenBXOrBinaryOp final : public CodegenBinaryOp {
public:
    CodegenBXOrBinaryOp(Codegen & cg,
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
