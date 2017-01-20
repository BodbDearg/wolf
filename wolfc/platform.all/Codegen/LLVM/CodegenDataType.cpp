#include "CodegenDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

CodegenDataType::CodegenDataType(CodegenCtx & ctx, ConstCodegen & constCodegen) :
    mCtx(ctx),
    mConstCodegen(constCodegen)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenDataType::visit(const ArrayDataType & dataType) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const BoolDataType & dataType) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const Int64DataType & dataType) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const StrDataType & dataType) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const UnknownArrayDataType & dataType) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const UnknownDataType & dataType) {
    #warning TODO: Codegen this data type
}

void CodegenDataType::visit(const VoidDataType & dataType) {
    #warning TODO: Codegen this data type
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
