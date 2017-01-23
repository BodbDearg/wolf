#include "WCGenericIntDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

bool GenericIntDataType::isInteger() const {
    return true;
}

#warning FIXME - Codegen
#if 0
llvm::Value * GenericIntDataType::codegenPlusOp(CodegenCtx & cgCtx,
                                                AST::ASTNode & callingNode,
                                                llvm::Value & val)
{
    // This is a 'no-op' for numbers - simply return the value passed in
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(callingNode);
    return &val;
}

llvm::Value * GenericIntDataType::codegenMinusOp(CodegenCtx & cgCtx,
                                                 AST::ASTNode & callingNode,
                                                 llvm::Value & val)
{
    #warning DO NOT ALLOW FOR UNSIGNED
    // FIXME: Should this be an error for unsigned integers?
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateNeg(&val);
}

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

llvm::Constant * GenericIntDataType::codegenConstAddOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstSubOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstBOrOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstBXOrOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstMulOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstBAndOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstLShiftOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstLRShiftOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericIntDataType::codegenConstBNotOp(AST::ASTNode & callingNode,
                                                        llvm::Constant & val)
{
    WC_UNUSED_PARAM(callingNode);
    return llvm::ConstantExpr::getNot(&val);
}

llvm::Constant * GenericIntDataType::codegenConstPlusOp(AST::ASTNode & callingNode,
                                                        llvm::Constant & val)
{
    // This is a 'no-op' for numbers - simply return the value passed in
    WC_UNUSED_PARAM(callingNode);
    return &val;
}

llvm::Constant * GenericIntDataType::codegenConstMinusOp(AST::ASTNode & callingNode,
                                                         llvm::Constant & val)
{
    #warning DO NOT ALLOW FOR UNSIGNED
    // FIXME: Should this be an error for unsigned integers?
    WC_UNUSED_PARAM(callingNode);
    return llvm::ConstantExpr::getNeg(&val);
}
#endif

WC_END_NAMESPACE
