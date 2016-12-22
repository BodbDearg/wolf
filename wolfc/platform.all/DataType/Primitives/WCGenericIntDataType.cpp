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
                                               DataType & rightTy,
                                               llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Add,
                                            kOpName_Add,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateAdd(&leftVal, &rightVal, "GenericInt_AddOp");
}

llvm::Value * GenericIntDataType::codegenSubOp(CodegenCtx & cgCtx,
                                               ASTNode & callingNode,
                                               llvm::Value & leftVal,
                                               DataType & rightTy,
                                               llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Sub,
                                            kOpName_Sub,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateSub(&leftVal, &rightVal, "GenericInt_SubOp");
}

llvm::Value * GenericIntDataType::codegenBOrOp(CodegenCtx & cgCtx,
                                               ASTNode & callingNode,
                                               llvm::Value & leftVal,
                                               DataType & rightTy,
                                               llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_BOr,
                                            kOpName_BOr,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateOr(&leftVal, &rightVal, "GenericInt_BOrOp");
}

llvm::Value * GenericIntDataType::codegenBXOrOp(CodegenCtx & cgCtx,
                                                ASTNode & callingNode,
                                                llvm::Value & leftVal,
                                                DataType & rightTy,
                                                llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_BXOr,
                                            kOpName_BXOr,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateXor(&leftVal, &rightVal, "GenericInt_BXOrOp");
}

llvm::Value * GenericIntDataType::codegenMulOp(CodegenCtx & cgCtx,
                                               ASTNode & callingNode,
                                               llvm::Value & leftVal,
                                               DataType & rightTy,
                                               llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Mul,
                                            kOpName_Mul,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateMul(&leftVal, &rightVal, "GenericInt_MulOp");
}

llvm::Value * GenericIntDataType::codegenBAndOp(CodegenCtx & cgCtx,
                                                ASTNode & callingNode,
                                                llvm::Value & leftVal,
                                                DataType & rightTy,
                                                llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_BAnd,
                                            kOpName_BAnd,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateAnd(&leftVal, &rightVal, "GenericInt_BAndOp");
}

llvm::Value * GenericIntDataType::codegenLShiftOp(CodegenCtx & cgCtx,
                                                  ASTNode & callingNode,
                                                  llvm::Value & leftVal,
                                                  DataType & rightTy,
                                                  llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_LShift,
                                            kOpName_LShift,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateShl(&leftVal, &rightVal, "GenericInt_LShiftOp");
}

llvm::Value * GenericIntDataType::codegenLRShiftOp(CodegenCtx & cgCtx,
                                                   ASTNode & callingNode,
                                                   llvm::Value & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_LRShift,
                                            kOpName_LRShift,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateLShr(&leftVal, &rightVal, "GenericInt_LRShiftOp");
}

llvm::Constant * GenericIntDataType::codegenConstAddOp(ASTNode & callingNode,
                                                       llvm::Constant & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Add,
                                            kOpName_Add,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getAdd(&leftVal, &rightVal);
}

llvm::Constant * GenericIntDataType::codegenConstSubOp(ASTNode & callingNode,
                                                       llvm::Constant & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Sub,
                                            kOpName_Sub,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getSub(&leftVal, &rightVal);
}

llvm::Constant * GenericIntDataType::codegenConstBOrOp(ASTNode & callingNode,
                                                       llvm::Constant & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_BOr,
                                            kOpName_BOr,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getOr(&leftVal, &rightVal);
}

llvm::Constant * GenericIntDataType::codegenConstBXOrOp(ASTNode & callingNode,
                                                        llvm::Constant & leftVal,
                                                        DataType & rightTy,
                                                        llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_BXOr,
                                            kOpName_BXOr,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getXor(&leftVal, &rightVal);
}

llvm::Constant * GenericIntDataType::codegenConstMulOp(ASTNode & callingNode,
                                                       llvm::Constant & leftVal,
                                                       DataType & rightTy,
                                                       llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_Mul,
                                            kOpName_Mul,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getMul(&leftVal, &rightVal);
}

llvm::Constant * GenericIntDataType::codegenConstBAndOp(ASTNode & callingNode,
                                                        llvm::Constant & leftVal,
                                                        DataType & rightTy,
                                                        llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_BAnd,
                                            kOpName_BAnd,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getAnd(&leftVal, &rightVal);
}

llvm::Constant * GenericIntDataType::codegenConstLShiftOp(ASTNode & callingNode,
                                                          llvm::Constant & leftVal,
                                                          DataType & rightTy,
                                                          llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_LShift,
                                            kOpName_LShift,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getShl(&leftVal, &rightVal);
}

llvm::Constant * GenericIntDataType::codegenConstLRShiftOp(ASTNode & callingNode,
                                                           llvm::Constant & leftVal,
                                                           DataType & rightTy,
                                                           llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_LRShift,
                                            kOpName_LRShift,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getLShr(&leftVal, &rightVal);
}

WC_END_NAMESPACE
