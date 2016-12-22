#include "WCGenericSignedIntDataType.hpp"

#include "AST/WCASTNode.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

llvm::Value * GenericSignedIntDataType::codegenCmpLTOp(CodegenCtx & cgCtx,
                                                       ASTNode & callingNode,
                                                       llvm::Value & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpLT,
                                            kOpName_CmpLT,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpSLT(&leftVal, &rightVal, "GenericSignedInt_CmpLTOp");
}

llvm::Value * GenericSignedIntDataType::codegenCmpLEOp(CodegenCtx & cgCtx,
                                                       ASTNode & callingNode,
                                                       llvm::Value & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpLE,
                                            kOpName_CmpLE,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpSLE(&leftVal, &rightVal, "GenericSignedInt_CmpLEOp");
}

llvm::Value * GenericSignedIntDataType::codegenCmpGTOp(CodegenCtx & cgCtx,
                                                       ASTNode & callingNode,
                                                       llvm::Value & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpGT,
                                            kOpName_CmpGT,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpSGT(&leftVal, &rightVal, "GenericSignedInt_CmpGTOp");
}

llvm::Value * GenericSignedIntDataType::codegenCmpGEOp(CodegenCtx & cgCtx,
                                                       ASTNode & callingNode,
                                                       llvm::Value & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpGE,
                                            kOpName_CmpGE,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpSGE(&leftVal, &rightVal, "GenericSignedInt_CmpGEOp");
}

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

llvm::Constant * GenericSignedIntDataType::codegenConstCmpLTOp(ASTNode & callingNode,
                                                               llvm::Constant & leftVal,
                                                               DataType & rightTy,
                                                               llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpLT,
                                            kOpName_CmpLT,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLT, &leftVal, &rightVal);
}

llvm::Constant * GenericSignedIntDataType::codegenConstCmpLEOp(ASTNode & callingNode,
                                                               llvm::Constant & leftVal,
                                                               DataType & rightTy,
                                                               llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpLE,
                                            kOpName_CmpLE,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLE, &leftVal, &rightVal);
}

llvm::Constant * GenericSignedIntDataType::codegenConstCmpGTOp(ASTNode & callingNode,
                                                               llvm::Constant & leftVal,
                                                               DataType & rightTy,
                                                               llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpGT,
                                            kOpName_CmpGT,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGT, &leftVal, &rightVal);
}

llvm::Constant * GenericSignedIntDataType::codegenConstCmpGEOp(ASTNode & callingNode,
                                                               llvm::Constant & leftVal,
                                                               DataType & rightTy,
                                                               llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpGE,
                                            kOpName_CmpGE,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGE, &leftVal, &rightVal);
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
