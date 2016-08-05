#include "WCCodegenCtx.hpp"
#include "WCAssert.hpp"
#include "WCASTNode.hpp"

WC_BEGIN_NAMESPACE

void CodegenCtx::pushInsertBlock() {
    insertBlockStack.push_back(irBuilder.GetInsertBlock());
    WC_ASSERT(insertBlockStack.back() != nullptr);
}

void CodegenCtx::popInsertBlock() {
    WC_ASSERT(!insertBlockStack.empty());
    llvm::BasicBlock * basicBlock = insertBlockStack.back();
    insertBlockStack.pop_back();
    irBuilder.SetInsertPoint(basicBlock);
}

bool CodegenCtx::irbCreateStore(ASTNode & callingNode,
                                llvm::Value * val,
                                llvm::Value * ptr)
{
    WC_ASSERT(val);
    WC_ASSERT(ptr);
    
    if (!irBuilder.CreateStore(val, ptr)) {
        callingNode.compileError("Failed to create an llvm store instruction!");
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
