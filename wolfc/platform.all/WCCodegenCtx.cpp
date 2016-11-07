#include "WCCodegenCtx.hpp"

#include "WCAssert.hpp"

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

WC_END_NAMESPACE
