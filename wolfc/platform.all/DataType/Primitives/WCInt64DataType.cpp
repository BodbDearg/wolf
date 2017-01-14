#include "WCInt64DataType.hpp"

#include "AST/WCASTNode.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

static const std::string kName = "int64";

DataTypeId Int64DataType::getTypeId() const {
    return DataTypeId::kInt64;
}

const std::string & Int64DataType::name() const {
    return kName;
}

bool Int64DataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const Int64DataType*>(&other) != nullptr;
}

bool Int64DataType::codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) {
    // Lazy out if already done
    WC_GUARD(!mLLVMType, true);
    mLLVMType = llvm::Type::getInt64Ty(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

bool Int64DataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                      AST::ASTNode & callingNode,
                                      llvm::Constant & printfFn,
                                      llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(callingNode);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%lld", "print_fmt_str:int");
    return cgCtx.irBuilder.CreateCall(&printfFn,
                                      { fmtStr, &valToPrint },
                                      "print_printf_call:int") != nullptr;
}

WC_END_NAMESPACE
