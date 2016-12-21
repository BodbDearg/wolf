#include "WCGenericIntDataType.hpp"

#include "AST/WCASTNode.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

bool GenericIntDataType::isInteger() const {
    return true;
}

llvm::Value * GenericIntDataType::codegenAddOp(CodegenCtx & cgCtx,
                                               ASTNode & callingNode,
                                               llvm::Value & leftVal,
                                               DataType & rightType,
                                               llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode, "+", "add", rightType), nullptr);
    return cgCtx.irBuilder.CreateAdd(&leftVal, &rightVal, "GenericInt_AddOp");
}

WC_END_NAMESPACE
