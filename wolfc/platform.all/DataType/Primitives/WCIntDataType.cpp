#include "WCIntDataType.hpp"
#include "WCAssert.hpp"
#include "WCASTNode.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

static const std::string kName = "int";

const std::string & IntDataType::name() const {
    return kName;
}

bool IntDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const IntDataType*>(&other) != nullptr;
}

bool IntDataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    // Lazy out if already done
    WC_GUARD(!mLLVMType, true);
    mLLVMType = llvm::Type::getInt64Ty(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

llvm::AllocaInst * IntDataType::codegenAlloca(CodegenCtx & cgCtx,
                                              ASTNode & callingNode,
                                              const std::string & instLabel)
{
    WC_GUARD_ASSERT(mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
}

bool IntDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                    const PrintStmnt & parentPrintStmnt,
                                    llvm::Constant & printfFn,
                                    llvm::Value & value) const
{
    WC_UNUSED_PARAM(parentPrintStmnt);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%zd", "print_fmt_str:int");
    return cgCtx.irBuilder.CreateCall(&printfFn, { fmtStr, &value }, "print_printf_call:int") != nullptr;
}

WC_END_NAMESPACE
