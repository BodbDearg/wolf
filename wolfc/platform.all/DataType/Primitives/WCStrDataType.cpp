#include "WCStrDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "string";

void StrDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId StrDataType::getTypeId() const {
    return DataTypeId::kStr;
}

const std::string & StrDataType::name() const {
    return kName;
}

bool StrDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const StrDataType*>(&other) != nullptr;
}

#warning FIXME - Codegen
#if 0
bool StrDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                    AST::ASTNode & callingNode,
                                    llvm::Constant & printfFn,
                                    llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(callingNode);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%s", "print_fmt_str:string");
    return cgCtx.irBuilder.CreateCall(&printfFn,
                                      { fmtStr, &valToPrint },
                                      "print_printf_call:string") != nullptr;
}
#endif

WC_END_NAMESPACE
