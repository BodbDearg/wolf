#include "WCBoolDataType.hpp"

#include "WCAssert.hpp"
#include "WCASTNode.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrintStmnt.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "bool";

const std::string & BoolDataType::name() const {
    return kName;
}

bool BoolDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const BoolDataType*>(&other) != nullptr;
}

bool BoolDataType::isBool() const {
    return true;
}

llvm::AllocaInst * BoolDataType::codegenAlloca(CodegenCtx & cgCtx,
                                               ASTNode & callingNode,
                                               const std::string & instLabel)
{
    WC_GUARD_ASSERT(mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
}

bool BoolDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                     const PrintStmnt & parentPrintStmnt,
                                     llvm::Constant & printfFn,
                                     llvm::Value & value) const
{
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(parentPrintStmnt);
    
    // Create the two blocks for true & false and a branch
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();

    llvm::BasicBlock * trueBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "print:true", parentFn);
    WC_GUARD_ASSERT(trueBB, false);
    llvm::BasicBlock * falseBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "print:false", parentFn);
    WC_GUARD_ASSERT(falseBB, false);
    
    // Generate the branch
    llvm::Value * branch = cgCtx.irBuilder.CreateCondBr(&value, trueBB, falseBB);
    WC_GUARD_ASSERT(branch, false);
    llvm::BasicBlock * continueBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "print:continue", parentFn);
    WC_GUARD_ASSERT(continueBB, false);
    
    // Generate code for print 'true' block
    {
        cgCtx.irBuilder.SetInsertPoint(trueBB);
        llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("true", "print_fmt_str:bool:true");
        WC_GUARD_ASSERT(fmtStr, false);
        WC_GUARD_ASSERT(cgCtx.irBuilder.CreateCall(&printfFn, fmtStr, "print_printf_call:bool:true"), false);
        WC_GUARD_ASSERT(cgCtx.irBuilder.CreateBr(continueBB), false);
    }
    
    // Generate code for print 'false' block
    {
        cgCtx.irBuilder.SetInsertPoint(falseBB);
        llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("false", "print_fmt_str:bool:false");
        WC_GUARD_ASSERT(fmtStr, false);
        WC_GUARD_ASSERT(cgCtx.irBuilder.CreateCall(&printfFn, fmtStr, "print_printf_call:bool:false"), false);
        WC_GUARD_ASSERT(cgCtx.irBuilder.CreateBr(continueBB), false);
    }
    
    // Restore the previous insert point
    cgCtx.irBuilder.SetInsertPoint(continueBB);
    return true;
}

bool BoolDataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    mLLVMType = llvm::Type::getInt1Ty(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
