#include "WCGenericSignedIntDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"

WC_BEGIN_NAMESPACE

#warning FIXME - Codegen
#if 0
llvm::Constant * GenericSignedIntDataType::codegenConstDivOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericSignedIntDataType::codegenConstModOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericSignedIntDataType::codegenConstARShiftOp(AST::ASTNode & callingNode,
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
#endif

WC_END_NAMESPACE
