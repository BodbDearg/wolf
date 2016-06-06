#include "WCVoidDataType.hpp"
#include "WCPrintStmnt.hpp"

WC_BEGIN_NAMESPACE

const char * VoidDataType::name() const {
    return "void";
}

bool VoidDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const VoidDataType*>(&other) != nullptr;
}

bool VoidDataType::codegenPrintStmnt(const CodegenCtx & cgCtx,
                                     const PrintStmnt & parentPrintStmnt,
                                     llvm::Constant & printfFn,
                                     llvm::Value & value) const
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(value);
    parentPrintStmnt.compileError("Type 'void' cannot be printed!");
    return false;
}

WC_END_NAMESPACE
