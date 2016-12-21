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

bool Int64DataType::isInteger() const {
    return true;
}

bool Int64DataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    // Lazy out if already done
    WC_GUARD(!mLLVMType, true);
    mLLVMType = llvm::Type::getInt64Ty(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

llvm::AllocaInst * Int64DataType::codegenAlloca(CodegenCtx & cgCtx,
                                                ASTNode & callingNode,
                                                const std::string & instLabel)
{
    WC_GUARD_ASSERT(mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
}

bool Int64DataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                      const PrintStmnt & parentPrintStmnt,
                                      llvm::Constant & printfFn,
                                      llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(parentPrintStmnt);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%lld", "print_fmt_str:int");
    return cgCtx.irBuilder.CreateCall(&printfFn,
                                      { fmtStr, &valToPrint },
                                      "print_printf_call:int") != nullptr;
}

llvm::Value * Int64DataType::codegenAddOp(CodegenCtx & cgCtx,
                                          ASTNode & callingNode,
                                          llvm::Value & leftVal,
                                          DataType & rightType,
                                          llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode, "+", "add", rightType), nullptr);
    return cgCtx.irBuilder.CreateAdd(&leftVal, &rightVal, "Int64_AddOp");
}

WC_END_NAMESPACE
