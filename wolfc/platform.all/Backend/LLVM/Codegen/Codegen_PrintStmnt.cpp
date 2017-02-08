#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/PrintStmnt.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* Generate the code for a print statement when we are dealing with a bool */
static void codegenBoolPrintStmnt(CodegenCtx & ctx,
                                  llvm::Constant & printfFn,
                                  llvm::Value & valToPrint)
{
    // Create the two blocks for true & false and a branch
    llvm::Function * parentFn = ctx.mIRBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    llvm::BasicBlock * trueBB = llvm::BasicBlock::Create(ctx.mLLVMCtx, "print:true", parentFn);
    WC_ASSERT(trueBB);
    llvm::BasicBlock * falseBB = llvm::BasicBlock::Create(ctx.mLLVMCtx, "print:false", parentFn);
    WC_ASSERT(falseBB);
    
    // Generate the branch
    llvm::Value * branch = ctx.mIRBuilder.CreateCondBr(&valToPrint, trueBB, falseBB);
    WC_ASSERT(branch);
    llvm::BasicBlock * continueBB = llvm::BasicBlock::Create(ctx.mLLVMCtx, "print:continue", parentFn);
    WC_ASSERT(continueBB);
    
    // Generate code for print 'true' block
    {
        ctx.mIRBuilder.SetInsertPoint(trueBB);
        llvm::Value * fmtStr = ctx.mIRBuilder.CreateGlobalStringPtr("true", "print:fmt_str");
        WC_ASSERT(fmtStr);
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateCall(&printfFn, fmtStr, "print:printf_call:true"));
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateBr(continueBB));
    }
    
    // Generate code for print 'false' block
    {
        ctx.mIRBuilder.SetInsertPoint(falseBB);
        llvm::Value * fmtStr = ctx.mIRBuilder.CreateGlobalStringPtr("false", "print:fmt_str");
        WC_ASSERT(fmtStr);
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateCall(&printfFn, fmtStr, "print:printf_call:false"));
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateBr(continueBB));
    }
    
    // Restore the previous insert point
    ctx.mIRBuilder.SetInsertPoint(continueBB);
}

/* Generate the code for a print statement for an arbitrary value */
static void codegenGenericValuePrintStmnt(CodegenCtx & ctx,
                                          llvm::Constant & printfFn,
                                          llvm::Value & valToPrint,
                                          const char * formatStr)
{
    // Create a format string for printf and call
    llvm::Value * fmtStr = ctx.mIRBuilder.CreateGlobalStringPtr(formatStr, "print:fmt_str");
    WC_ASSERT(fmtStr);
    WC_ASSERTED_OP(ctx.mIRBuilder.CreateCall(&printfFn,
                                             { fmtStr, &valToPrint },
                                             "print:printf_call"));
}

void Codegen::visit(const AST::PrintStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Get printf
    llvm::Constant * printfFn = mCtx.mLLVMModule->getFunction("printf");
    
    if (!printfFn) {
        mCtx.error(astNode, "Codegen failed! Can't find 'printf' function!");
    }
    
    // Evaluate the code for the argument to printf
    astNode.mExpr.accept(*this);
    Value exprVal = mCtx.popValue();
    
    // Can only generate the print call if we have both printf and an expression value
    if (printfFn && exprVal.isValid()) {
        // See which type we are dealing with:
        const DataType & exprDataType = exprVal.mCompiledType.getDataType();
        
        switch (exprDataType.getTypeId()) {
            case DataTypeId::kInt64:
                codegenGenericValuePrintStmnt(mCtx, *printfFn, *exprVal.mLLVMVal, "%lld");
                break;
                
            case DataTypeId::kBool:
                codegenBoolPrintStmnt(mCtx, *printfFn, *exprVal.mLLVMVal);
                break;
                
            case DataTypeId::kStr:
                codegenGenericValuePrintStmnt(mCtx, *printfFn, *exprVal.mLLVMVal, "%s");
                break;
                
            case DataTypeId::kInvalid:
            case DataTypeId::kVoid:
            case DataTypeId::kArray:
            case DataTypeId::kFunc:
                mCtx.error(astNode,
                           "print() not supported/implemented for expression of type '%s'!",
                           exprDataType.name().c_str());
                break;
        }
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
