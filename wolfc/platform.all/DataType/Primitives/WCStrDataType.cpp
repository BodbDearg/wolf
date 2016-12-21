#include "WCStrDataType.hpp"

#include "AST/WCASTNode.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

static const std::string kName = "string";

DataTypeId StrDataType::getTypeId() const {
    return DataTypeId::kStr;
}

const std::string & StrDataType::name() const {
    return kName;
}

bool StrDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const StrDataType*>(&other) != nullptr;
}

llvm::AllocaInst * StrDataType::codegenAlloca(CodegenCtx & cgCtx,
                                              ASTNode & callingNode,
                                              const std::string & instLabel)
{
    WC_GUARD_ASSERT(mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
}

bool StrDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                    const PrintStmnt & parentPrintStmnt,
                                    llvm::Constant & printfFn,
                                    llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(parentPrintStmnt);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%s", "print_fmt_str:string");
    return cgCtx.irBuilder.CreateCall(&printfFn,
                                      { fmtStr, &valToPrint },
                                      "print_printf_call:string") != nullptr;
}

bool StrDataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    mLLVMType = llvm::Type::getInt8PtrTy(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
