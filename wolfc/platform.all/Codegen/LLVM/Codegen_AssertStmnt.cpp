#include "Codegen.hpp"

#include "AST/Nodes/WCAssertStmnt.hpp"
#include "AST/Nodes/WCAssignExpr.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

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
    llvm::Value * exprVal = mCtx.popLLVMValue();
    
    // The expression being asserted must evaluate to a boolean
    const DataType & exprDataType = astNode.mExpr.dataType();
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
    WC_GUARD(abortFn && printfFn && exprVal && exprIsBool);
    
    // Create basic blocks for assert failed and assert succeeded:
    llvm::BasicBlock * failBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, "AssertStmnt:fail", parentFn);
    WC_ASSERT(failBB);
    llvm::BasicBlock * passBB = llvm::BasicBlock::Create(mCtx.mLLVMCtx, "AssertStmnt:pass", parentFn);
    WC_ASSERT(passBB);
    
    // Create a branch to one of the two blocks:
    WC_ASSERTED_OP(irb.CreateCondBr(exprVal, passBB, failBB));
    
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

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
