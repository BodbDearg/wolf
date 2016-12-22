#include "WCGenericSignedIntDataType.hpp"

#include "AST/WCASTNode.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

llvm::Value * GenericSignedIntDataType::codegenDivOp(CodegenCtx & cgCtx,
                                                     ASTNode & callingNode,
                                                     llvm::Value & leftVal,
                                                     DataType & rightTy,
                                                     llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Div,
                                            kOpName_Div,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateSDiv(&leftVal, &rightVal, "GenericSignedInt_DivOp");
}

llvm::Value * GenericSignedIntDataType::codegenModOp(CodegenCtx & cgCtx,
                                                     ASTNode & callingNode,
                                                     llvm::Value & leftVal,
                                                     DataType & rightTy,
                                                     llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Mod,
                                            kOpName_Mod,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateSRem(&leftVal, &rightVal, "GenericSignedInt_ModOp");
}

llvm::Value * GenericSignedIntDataType::codegenARShiftOp(CodegenCtx & cgCtx,
                                                         ASTNode & callingNode,
                                                         llvm::Value & leftVal,
                                                         DataType & rightTy,
                                                         llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_ARShift,
                                            kOpName_ARShift,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateAShr(&leftVal, &rightVal, "GenericSignedInt_ARShiftOp");
}

llvm::Constant * GenericSignedIntDataType::codegenConstDivOp(ASTNode & callingNode,
                                                             llvm::Constant & leftVal,
                                                             DataType & rightTy,
                                                             llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Div,
                                            kOpName_Div,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getSDiv(&leftVal, &rightVal);
}

llvm::Constant * GenericSignedIntDataType::codegenConstModOp(ASTNode & callingNode,
                                                             llvm::Constant & leftVal,
                                                             DataType & rightTy,
                                                             llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Mod,
                                            kOpName_Mod,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getSRem(&leftVal, &rightVal);
}

llvm::Constant * GenericSignedIntDataType::codegenConstARShiftOp(ASTNode & callingNode,
                                                                 llvm::Constant & leftVal,
                                                                 DataType & rightTy,
                                                                 llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_ARShift,
                                            kOpName_ARShift,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getAShr(&leftVal, &rightVal);
}

WC_END_NAMESPACE
