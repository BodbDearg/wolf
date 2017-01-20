#include "CodegenDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenDataType::CodegenDataType(CodegenCtx & ctx, ConstCodegen & constCodegen) :
    mCtx(ctx),
    mConstCodegen(constCodegen)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenDataType::visit(const ArrayDataType & node) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const BoolDataType & node) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const Int64DataType & node) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const StrDataType & node) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const UnknownArrayDataType & node) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const UnknownDataType & node) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const VoidDataType & node) {
    #warning TODO: Codegen this data type
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
