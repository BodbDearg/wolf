#include "WCStrDataType.hpp"
#include "WCAssert.hpp"
#include "WCASTNode.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

static const std::string kName = "string";

const std::string & StrDataType::name() const {
    return kName;
}

bool StrDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const StrDataType*>(&other) != nullptr;
}

bool StrDataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    mLLVMType = llvm::Type::getInt8PtrTy(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        callingNode.compileError("Failed to generate llvm type for data type '%s'!",
                                 name().c_str());
        
        return false;
    }
    
    return true;
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
                                    llvm::Value & value) const
{
    WC_UNUSED_PARAM(parentPrintStmnt);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%s", "print_fmt_str:string");
    return cgCtx.irBuilder.CreateCall(&printfFn, { fmtStr, &value }, "print_printf_call:string") != nullptr;
}

WC_END_NAMESPACE
