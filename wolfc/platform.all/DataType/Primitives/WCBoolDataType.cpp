#include "WCBoolDataType.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrintStmnt.hpp"

WC_BEGIN_NAMESPACE

const char * BoolDataType::name() const {
    return "bool";
}

bool BoolDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const BoolDataType*>(&other) != nullptr;
}

bool BoolDataType::isSized() const {
    return true;
}

bool BoolDataType::isBool() const {
    return true;
}

llvm::Type * BoolDataType::llvmType(CodegenCtx & cgCtx) const {
    return llvm::Type::getInt1Ty(cgCtx.llvmCtx);
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

WC_END_NAMESPACE
