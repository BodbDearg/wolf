//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/AssertStmnt.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::AssertStmnt & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Get 'abort' and 'printf' c library functions
    llvm::Constant * abortFn = mCtx.mLLVMModule->getFunction("abort");
    llvm::Constant * printfFn = mCtx.mLLVMModule->getFunction("printf");
    
    if (!abortFn) {
        mCtx.error(astNode, "Can't find 'abort' C library function for assert statement!");
    }
    
    if (!printfFn) {
        mCtx.error(astNode, "Can't find 'printf' C library function for assert statement!");
    }
    
    // Evaluate the value for the expression being asserted
    astNode.mExpr.accept(*this);
    Value exprVal = mCtx.popValue();
    
    // If that failed don't issue any further errors, bail:
    WC_GUARD(exprVal.isValid());
    
    // Codegen the bool data type and do any implicit casts required to bool
    PrimitiveDataTypes::getBoolDataType().accept(mCodegenDataType);
    CompiledDataType boolCDT = mCtx.popCompiledDataType();
    ImplicitCasts::castSingleValueIfRequired(*this, exprVal, boolCDT);
    
    // If that failed don't issue any further errors, bail:
    WC_GUARD(exprVal.isValid());
    
    // The expression being asserted must evaluate to a boolean.
    const DataType & exprDataType = exprVal.mCompiledType.getDataType();
    bool exprIsBool = exprDataType.isBool();
    
    if (!exprIsBool) {
        mCtx.error(astNode.mExpr,
                   "Expression being asserted must evaluate to type 'bool', not '%s'!",
                   exprDataType.name().c_str());
    }
    
    // Grab the parent function
    auto & irb = mCtx.mIRBuilder;
    llvm::Function * parentFn = irb.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Okay, proceed no further if any of these are bad
    WC_GUARD(abortFn && printfFn && exprVal.isValid() && exprIsBool);
    
    // Create basic blocks for assert failed and assert succeeded:
    std::string failBBLbl = StringUtils::appendLineInfo("AssertStmnt:fail", astNode.getStartToken());
    llvm::BasicBlock * failBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, failBBLbl, parentFn);
    WC_ASSERT(failBB);
    
    std::string passBBLbl = StringUtils::appendLineInfo("AssertStmnt:pass", astNode.getPastEndToken());
    llvm::BasicBlock * passBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, passBBLbl, parentFn);
    WC_ASSERT(passBB);
    
    // Create a branch to one of the two blocks:
    WC_ASSERT(exprVal.mLLVMVal);
    WC_ASSERTED_OP(irb.CreateCondBr(exprVal.mLLVMVal, passBB, failBB));
    
    // Makeup the string for the assert failed message:
    std::string assertMsgStr;
    
    {
        const Token & startTok = astNode.mExpr.getStartToken();
        const Token & endTok = astNode.mExpr.getEndToken();
        size_t exprCodeLength = static_cast<size_t>(endTok.endSrcPtr - startTok.startSrcPtr);
        assertMsgStr.reserve(128 + exprCodeLength);
        assertMsgStr += "Assert failed on line ";
        assertMsgStr += std::to_string(startTok.startLine + 1);
        assertMsgStr += " char ";
        assertMsgStr += std::to_string(startTok.startCol + 1);;
        assertMsgStr += "!: assert(";
        assertMsgStr.append(startTok.startSrcPtr, exprCodeLength);
        assertMsgStr += ")\n";
    }
    
    // Create a string constant for the assert message:
    llvm::Value * assertMsgStrVal = irb.CreateGlobalStringPtr(assertMsgStr, "AssertStmnt:msg");
    WC_ASSERT(assertMsgStrVal);
    
    // Create a format string for printf:
    llvm::Value * printfFmtStrVal = irb.CreateGlobalStringPtr("%s", "AssertStmnt:printf_fmt_str");
    WC_ASSERT(printfFmtStrVal);
    
    // Generate code for assert fail
    irb.SetInsertPoint(failBB);
    WC_ASSERTED_OP(irb.CreateCall(printfFn, { printfFmtStrVal, assertMsgStrVal }, "AssertStmnt:printf:result"));
    WC_ASSERTED_OP(irb.CreateCall(abortFn));
    WC_ASSERTED_OP(irb.CreateUnreachable());
    
    // Continue generating the rest of the code after assert success
    irb.SetInsertPoint(passBB);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
