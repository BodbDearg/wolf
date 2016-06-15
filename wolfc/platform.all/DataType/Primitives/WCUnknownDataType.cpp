#include "WCUnknownDataType.hpp"
#include "WCPrintStmnt.hpp"

WC_BEGIN_NAMESPACE

const char * UnknownDataType::name() const {
    return "unknown";
}

bool UnknownDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const UnknownDataType*>(&other) != nullptr;
}

llvm::Type * UnknownDataType::llvmType(CodegenCtx & cgCtx) const {
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

bool UnknownDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                        const PrintStmnt & parentPrintStmnt,
                                        llvm::Constant & printfFn,
                                        llvm::Value & value) const
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(value);
    parentPrintStmnt.compileError("Unknown datatype cannot be printed!");
    return false;
}

WC_END_NAMESPACE
