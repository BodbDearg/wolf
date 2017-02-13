#pragma once

#include "CodegenUnaryOp.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class Type;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class CodegenCastUnaryOp : public CodegenUnaryOp {
public:
    CodegenCastUnaryOp(Codegen & cg,
                       const AST::ASTNode & fromExpr,
                       const AST::Type & toType,
                       bool storeResultInExpr);
    
    virtual void codegen() override;
    
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const FuncDataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
private:
    const AST::Type &   mToType;
    CompiledDataType    mToTypeCDT;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
