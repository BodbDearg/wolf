#pragma once

#include "DataType/DataTypeVisitor.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenCtx;
class ConstCodegen;

/* Generates code for the LLVM backend */
class CodegenDataType : public DataTypeVisitor {
public:
    CodegenDataType(CodegenCtx & ctx, ConstCodegen & constCodegen);
    
    /* DataType visitor functions */
    virtual void visit(const ArrayDataType & node) override;
    virtual void visit(const BoolDataType & node) override;
    virtual void visit(const Int64DataType & node) override;
    virtual void visit(const StrDataType & node) override;
    virtual void visit(const UnknownArrayDataType & node) override;
    virtual void visit(const UnknownDataType & node) override;
    virtual void visit(const VoidDataType & node) override;
    
private:
    CodegenCtx &    mCtx;
    ConstCodegen &  mConstCodegen;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
