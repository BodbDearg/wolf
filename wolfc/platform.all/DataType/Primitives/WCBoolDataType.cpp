#include "WCBoolDataType.hpp"
#include "WCPrintStmnt.hpp"

WC_BEGIN_NAMESPACE

const char * BoolDataType::name() const {
    return "bool";
}

bool BoolDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const BoolDataType*>(&other) != nullptr;
}

llvm::Value * BoolDataType::genPrintStmntCode(const CodegenCtx & cgCtx,
                                              const PrintStmnt & parentPrintStmnt,
                                              llvm::Constant & printfFn,
                                              llvm::Value & value) const
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(value);
    parentPrintStmnt.compileError("Type 'bool' cannot be printed!");
    return nullptr;
}

WC_END_NAMESPACE
