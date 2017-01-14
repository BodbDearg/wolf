#include "WCBoolDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCPrintStmnt.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "bool";

DataTypeId BoolDataType::getTypeId() const {
    return DataTypeId::kBool;
}

const std::string & BoolDataType::name() const {
    return kName;
}

bool BoolDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const BoolDataType*>(&other) != nullptr;
}

bool BoolDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                     AST::ASTNode & callingNode,
                                     llvm::Constant & printfFn,
                                     llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(callingNode);
    WC_UNUSED_PARAM(printfFn);
    
    // Create the two blocks for true & false and a branch
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();

    llvm::BasicBlock * trueBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "print:true", parentFn);
    WC_GUARD_ASSERT(trueBB, false);
    llvm::BasicBlock * falseBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "print:false", parentFn);
    WC_GUARD_ASSERT(falseBB, false);
    
    // Generate the branch
    llvm::Value * branch = cgCtx.irBuilder.CreateCondBr(&valToPrint, trueBB, falseBB);
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

bool BoolDataType::codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) {
    mLLVMType = llvm::Type::getInt1Ty(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

llvm::Value * BoolDataType::codegenCmpEQOp(CodegenCtx & cgCtx,
                                           AST::ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpEQ,
                                            kOpName_CmpEQ,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpEQ(&leftVal, &rightVal, "Bool_CmpEQOp");
}

llvm::Value * BoolDataType::codegenCmpNEOp(CodegenCtx & cgCtx,
                                           AST::ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpNE,
                                            kOpName_CmpNE,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpNE(&leftVal, &rightVal, "Bool_CmpNEOp");
}

llvm::Constant * BoolDataType::codegenConstCmpEQOp(AST::ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpEQ,
                                            kOpName_CmpEQ,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_EQ, &leftVal, &rightVal);
}

llvm::Constant * BoolDataType::codegenConstCmpNEOp(AST::ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpNE,
                                            kOpName_CmpNE,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_NE, &leftVal, &rightVal);
}

WC_END_NAMESPACE
