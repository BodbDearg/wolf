#include "Codegen.hpp"

#include "AST/Nodes/WCAssignExpr.hpp"
#include "AST/Nodes/WCPrintStmnt.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

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
        llvm::Value * fmtStr = ctx.mIRBuilder.CreateGlobalStringPtr("true", "print_fmt_str:bool:true");
        WC_ASSERT(fmtStr);
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateCall(&printfFn, fmtStr, "print_printf_call:bool:true"));
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateBr(continueBB));
    }
    
    // Generate code for print 'false' block
    {
        ctx.mIRBuilder.SetInsertPoint(falseBB);
        llvm::Value * fmtStr = ctx.mIRBuilder.CreateGlobalStringPtr("false", "print_fmt_str:bool:false");
        WC_ASSERT(fmtStr);
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateCall(&printfFn, fmtStr, "print_printf_call:bool:false"));
        WC_ASSERTED_OP(ctx.mIRBuilder.CreateBr(continueBB));
    }
    
    // Restore the previous insert point
    ctx.mIRBuilder.SetInsertPoint(continueBB);
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
    llvm::Value * exprValue = mCtx.popLLVMValue();
    
    // Can only generate the print call if we have both printf and an expression value
    if (printfFn && exprValue) {
        // Get the data type of the expression:
        #warning Eval data type first
        DataType & exprDataType = astNode.mExpr.dataType();
        
        // See which type we are dealing with:
        switch (exprDataType.getTypeId()) {
            case DataTypeId::kBool:
                codegenBoolPrintStmnt(mCtx, *printfFn, *exprValue);
                break;
                
            #warning TODO: other types
                
            default:
                mCtx.error(astNode,
                           "print() not supported/implemented for expression of type '%s'!",
                           exprDataType.name().c_str());
                break;
        }
    }
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
