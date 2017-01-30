#include "WhileStmnt.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Assert.hpp"
#include "AssignExpr.hpp"
#include "DataType/DataType.hpp"
#include "LinearAlloc.hpp"
#include "Scope.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool WhileStmnt::peek(const Token * tokenPtr) {
    TokenType tokenType = tokenPtr->type;
    return tokenType == TokenType::kWhile || tokenType == TokenType::kUntil;
}

WhileStmnt * WhileStmnt::parse(ParseCtx & parseCtx) {
    // Parse the initial 'while' or 'until' keyword
    if (!peek(parseCtx.tok())) {
        parseCtx.error("While statement expected!");
        return nullptr;
    }
    
    // Skip the 'while' or 'until' token and save location
    const Token * startToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Parse the while expression (while condition):
    AssignExpr * whileExpr = AssignExpr::parse(parseCtx);
    WC_GUARD(whileExpr, nullptr);
    
    // See if there is a 'do' following. This keyword is optional, unless the body scope is required
    // to be on the same line as the enclosing while statement:
    bool bodyScopeRequiresNL = true;
    
    if (parseCtx.tok()->type == TokenType::kDo) {
        // Found a 'do' token, skip it. The body scope is allowed to be on the same line
        parseCtx.nextTok();
        bodyScopeRequiresNL = false;
    }
    
    // Expect scope following:
    Scope * bodyScope = Scope::parse(parseCtx);
    WC_GUARD(bodyScope, nullptr);
    
    // See if it violates newline rules:
    if (bodyScopeRequiresNL) {
        if (bodyScope->getStartToken().startLine == whileExpr->getEndToken().endLine) {
            parseCtx.error(bodyScope->getStartToken(),
                           "Code following 'while/until' statement condition must be on a new line unless "
                           "'do' is used after the condition.");
            
            return nullptr;
        }
    }
    
    // While block should be terminated by an 'end' token:
    if (parseCtx.tok()->type != TokenType::kEnd) {
        parseCtx.error("'end' expected to terminate 'while/until' block!");
        return nullptr;
    }
    
    // Skip 'end' token and save location
    const Token * endToken = parseCtx.tok();
    parseCtx.nextTok();
    
    // Done: return the parsed statement
    return WC_NEW_AST_NODE(parseCtx, WhileStmnt, *whileExpr, *bodyScope, *startToken, *endToken);
}

WhileStmnt::WhileStmnt(AssignExpr & whileExpr,
                       Scope & bodyScope,
                       const Token & startToken,
                       const Token & endToken)
:
    mWhileExpr(whileExpr),
    mBodyScope(bodyScope),
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_ASSERT(mStartToken.type == TokenType::kWhile || mStartToken.type == TokenType::kUntil);
    mWhileExpr.mParent = this;
    mBodyScope.mParent = this;
}

void WhileStmnt::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & WhileStmnt::getStartToken() const {
    return mStartToken;
}

const Token & WhileStmnt::getEndToken() const {
    return mEndToken;
}

bool WhileStmnt::allCodepathsHaveUncondRet() const {
    return false;
}

#warning FIXME - Codegen
#if 0
llvm::BasicBlock * WhileStmnt::getNextStmntTargetBlock() {
    return mWhileCondBB;
}

llvm::BasicBlock * WhileStmnt::getBreakStmntTargetBlock() {
    return mEndBB;
}

bool WhileStmnt::codegen(CodegenCtx & cgCtx) {
    // Grab the parent function
    llvm::Function * parentFn = cgCtx.irBuilder.GetInsertBlock()->getParent();
    WC_GUARD_ASSERT(parentFn, false);
    
    // Create the 'while' basic block that does the condition check.
    // Note: also make the previous block branch to this block in order to properly terminate it.
    mWhileCondBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "WhileStmnt:while_cond", parentFn);
    WC_ASSERT(mWhileCondBB);
    cgCtx.irBuilder.CreateBr(mWhileCondBB);
    
    // Generate the code for the while statement condition expression:
    cgCtx.irBuilder.SetInsertPoint(mWhileCondBB);
    llvm::Value * whileExprResult = codegenWhileExpr(cgCtx);
    WC_GUARD(whileExprResult, false);
    
    // Create the 'body' basic block:
    llvm::BasicBlock * bodyBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "WhileStmnt:body", parentFn);
    WC_ASSERT(bodyBB);

    // Codegen the 'body' block, this will go back up to the while condition block when done
    cgCtx.irBuilder.SetInsertPoint(bodyBB);
    
    if (!mBodyScope.codegen(cgCtx)) {
        return false;
    }
    
    cgCtx.irBuilder.CreateBr(mWhileCondBB);
    
    // Create the end basic block:
    mEndBB = llvm::BasicBlock::Create(cgCtx.llvmCtx, "WhileStmnt:end", parentFn);
    WC_ASSERT(mEndBB);
    
    // Generate the branch for the while condition
    cgCtx.irBuilder.SetInsertPoint(mWhileCondBB);
    llvm::Value * branch = nullptr;
    
    if (isWhileExprInversed()) {
        branch = cgCtx.irBuilder.CreateCondBr(whileExprResult, mEndBB, bodyBB);
    }
    else {
        branch = cgCtx.irBuilder.CreateCondBr(whileExprResult, bodyBB, mEndBB);
    }
    
    WC_ASSERT(branch);
    
    // Insert code after the end block from here on in
    cgCtx.irBuilder.SetInsertPoint(mEndBB);
    return true;
}
#endif

bool WhileStmnt::isWhileExprInversed() const {
    return mStartToken.type == TokenType::kUntil;
}

#warning FIXME - Codegen
#if 0
llvm::Value * WhileStmnt::codegenWhileExpr(CodegenCtx & cgCtx) const {
    // Firstly validate that the while statement condition expression is a bool:
    const DataType & whileExprDataType = mWhileExpr.dataType();
    
    if (!whileExprDataType.isBool()) {
        compileError("Condition for while statement must evaluate to type 'bool', not '%s'!",
                     whileExprDataType.name().c_str());
        
        return nullptr;
    }
    
    // Then generate the code
    return mWhileExpr.codegenExprEval(cgCtx);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE