#pragma once

#include "DataType/DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class Func;
    class FuncArg;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenCtx;
class ConstCodegen;

/* Generates code for the LLVM backend */
class CodegenDataType final : public DataTypeVisitor {
public:
    CodegenDataType(CodegenCtx & ctx, ConstCodegen & constCodegen);
    
    /* Generate the data type for a function and push it to the stack */
    virtual void visit(const AST::Func & func);
    virtual void visit(const AST::FuncArg & funcArg);
    
    /* DataType visitor functions */
    virtual void visit(const ArrayBadSizeDataType & dataType) override;
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const ArrayUnevalSizeDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const FuncDataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const InvalidDataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
private:
    CodegenCtx &    mCtx;
    ConstCodegen &  mConstCodegen;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
