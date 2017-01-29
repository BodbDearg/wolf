#pragma once

#include "CodegenConstUnaryOp.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class CodegenConstLNotUnaryOp : public CodegenConstUnaryOp {
public:
    CodegenConstLNotUnaryOp(ConstCodegen & cg,
                            const AST::ASTNode & expr);
    
    void visit(const BoolDataType & dataType) override;
};

class CodegenConstBNotUnaryOp : public CodegenConstUnaryOp {
public:
    CodegenConstBNotUnaryOp(ConstCodegen & cg,
                            const AST::ASTNode & expr);
    
    void visit(const Int64DataType & dataType) override;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
