#include "WCAssertStmnt.hpp"

#include "DataType/WCDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool AssertStmnt::peek(const Token * tokenPtr) {
    return tokenPtr[0].type == TokenType::kAssert;
}

AssertStmnt * AssertStmnt::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (tokenPtr->type != TokenType::kAssert) {
        parseError(*tokenPtr, "Expected keyword 'assert' for 'assert()' statement!");
        return nullptr;
    }
    
    const Token * assertTok = tokenPtr;
    ++tokenPtr;     // Consume 'assert'
    
    if (tokenPtr->type != TokenType::kLParen) {
        parseError(*tokenPtr, "Expected '(' following 'assert'!");
        return nullptr;
    }
    
    ++tokenPtr;     // Consume '('
    
    // Parse the inner expression
    AssignExpr * assignExpr = AssignExpr::parse(tokenPtr, alloc);
    WC_GUARD(assignExpr, nullptr);
    
    // Expect ')' following all that:
    if (tokenPtr->type != TokenType::kRParen) {
        parseError(*tokenPtr, "Expected closing ')' for 'assert()' statement!");
        return nullptr;
    }
    
    // Consume closing ')' and return parsed expression
    const Token * closingParenTok = tokenPtr;
    ++tokenPtr;
    
    // Create and return the print statement
    return WC_NEW_AST_NODE(alloc, AssertStmnt, *assertTok, *assignExpr, *closingParenTok);
}

AssertStmnt::AssertStmnt(const Token & startToken, AssignExpr & expr, const Token & endToken) :
    mStartToken(startToken),
    mExpr(expr),
    mEndToken(endToken)
{
    mExpr.mParent = this;
}

const Token & AssertStmnt::getStartToken() const {
    return mStartToken;
}

const Token & AssertStmnt::getEndToken() const {
    return mEndToken;
}

bool AssertStmnt::codegen(CodegenCtx & cgCtx) {
    // Get 'abort' c library function
    llvm::Constant * abortFn = cgCtx.module.getLLVMModuleRef().getFunction("abort");
    
    if (!abortFn) {
        compileError("Codegen failed! Can't find 'abort' function!");
        return false;
    }
    
    // Get 'printf' c library function
    llvm::Constant * printfFn = cgCtx.module.getLLVMModuleRef().getFunction("printf");
    
    if (!printfFn) {
        compileError("Codegen failed! Can't find 'printf' function!");
        return false;
    }
    
    // The expression being asserted must evaluate to a boolean
    const DataType & exprDataType = mExpr.dataType();
    
    if (!exprDataType.isBool()) {
        compileError("Expression being asserted must evaluate to type 'bool', not '%s'!",
                     exprDataType.name().c_str());
        
        return false;
    }
    
    // Evaluate the value for the expression being asserted
    llvm::Value * exprVal = mExpr.codegenExprEval(cgCtx);
    WC_GUARD(exprVal, false);
    
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_ASSERT(parentFn);
    
    // Create basic blocks for assert failed and assert succeeded:
    llvm::BasicBlock * failBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "AssertStmnt:fail", parentFn);
    WC_ASSERT(failBB);
    llvm::BasicBlock * passBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "AssertStmnt:pass", parentFn);
    WC_ASSERT(passBB);
    
    // Create a branch to one of the two blocks:
    WC_ASSERTED_OP(cgCtx.irBuilder.CreateCondBr(exprVal, passBB, failBB));
    
    // Makeup the string for the assert failed message:
    std::string assertMsgStr;
    
    {
        const Token & startTok = mExpr.getStartToken();
        const Token & endTok = mExpr.getEndToken();
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
    llvm::Value * assertMsgStrValue = cgCtx.irBuilder.CreateGlobalStringPtr(assertMsgStr.c_str(), "AssertMsg");
    WC_ASSERT(assertMsgStrValue);
    
    // Create a format string for printf:
    llvm::Value * printfFmtStrValue = cgCtx.irBuilder.CreateGlobalStringPtr("%s", "AssertPrintfFmtStr");
    WC_ASSERT(printfFmtStrValue);
    
    // Generate code for assert fail:
    cgCtx.irBuilder.SetInsertPoint(failBB);
    WC_ASSERTED_OP(cgCtx.irBuilder.CreateCall(printfFn, { printfFmtStrValue, assertMsgStrValue }));
    WC_ASSERTED_OP(cgCtx.irBuilder.CreateCall(abortFn));
    WC_ASSERTED_OP(cgCtx.irBuilder.CreateUnreachable());
    
    // Continue generating the rest of the code after assert success
    cgCtx.irBuilder.SetInsertPoint(passBB);
    return true;
}

WC_END_NAMESPACE
