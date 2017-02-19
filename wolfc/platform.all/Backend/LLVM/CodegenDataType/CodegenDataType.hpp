#pragma once

#include "DataType/DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class Func;
    class FuncArg;
    class PrimitiveType;
    class TypeArray;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenCtx;
class ConstCodegen;

/* Generates code for the LLVM backend */
class CodegenDataType final : public DataTypeVisitor {
public:
    CodegenDataType(CodegenCtx & ctx, ConstCodegen & constCodegen);
    
    /* Generate the data type for these AST nodes and push it to the codegen context stack */
    void visitASTNode(const AST::Func & func);
    void visitASTNode(const AST::FuncArg & funcArg);
    void visitASTNode(const AST::PrimitiveType & primitiveType);
    void visitASTNode(const AST::TypeArray & typeArray);
    
    /* DataType visitor functions */
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const FuncDataType & dataType) override;
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const InvalidDataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
private:
    CodegenCtx &    mCtx;
    ConstCodegen &  mConstCodegen;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
