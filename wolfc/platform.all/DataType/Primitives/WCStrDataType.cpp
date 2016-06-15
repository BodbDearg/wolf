#include "WCStrDataType.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

const char * StrDataType::name() const {
    return "string";
}

bool StrDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const StrDataType*>(&other) != nullptr;
}

llvm::Type * StrDataType::llvmType(CodegenCtx & cgCtx) const {
    return llvm::Type::getInt8PtrTy(cgCtx.llvmCtx);
}

bool StrDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                    const PrintStmnt & parentPrintStmnt,
                                    llvm::Constant & printfFn,
                                    llvm::Value & value) const
{
    WC_UNUSED_PARAM(parentPrintStmnt);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%s", "print_fmt_str:string");
    return cgCtx.irBuilder.CreateCall(&printfFn, { fmtStr, &value }, "print_printf_call:string") != nullptr;
}

WC_END_NAMESPACE
