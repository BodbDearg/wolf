#include "GenericIntDataType.hpp"

WC_BEGIN_NAMESPACE

bool GenericIntDataType::isInteger() const {
    return true;
}

#warning FIXME - Codegen
#if 0
llvm::Value * GenericIntDataType::codegenIncOp(CodegenCtx & cgCtx,
                                               AST::ASTNode & callingNode,
                                               llvm::Value & val)
{
    WC_UNUSED_PARAM(callingNode);
    llvm::Value * incBy = cgCtx.irBuilder.getInt64(1);
    WC_ASSERT(incBy);
    return cgCtx.irBuilder.CreateAdd(&val, incBy, "GenericInt_IncOp");
}

llvm::Value * GenericIntDataType::codegenDecOp(CodegenCtx & cgCtx,
                                               AST::ASTNode & callingNode,
                                               llvm::Value & val)
{
    WC_UNUSED_PARAM(callingNode);
    llvm::Value * decBy = cgCtx.irBuilder.getInt64(1);
    WC_ASSERT(decBy);
    return cgCtx.irBuilder.CreateSub(&val, decBy, "GenericInt_DecOp");
}

llvm::Constant * GenericIntDataType::codegenConstBNotOp(AST::ASTNode & callingNode,
                                                        llvm::Constant & val)
{
    WC_UNUSED_PARAM(callingNode);
    return llvm::ConstantExpr::getNot(&val);
}
#endif

WC_END_NAMESPACE
