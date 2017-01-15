#include "WCGenericSignedIntDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"

#warning FIXME - Codegen
#if 0
#include "WCCodegenCtx.hpp"
#endif

#warning FIXME - Codegen
#if 0
WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END
#endif

WC_BEGIN_NAMESPACE

#warning FIXME - Codegen
#if 0
llvm::Value * GenericSignedIntDataType::codegenCmpLTOp(CodegenCtx & cgCtx,
                                                       AST::ASTNode & callingNode,
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
                                                       AST::ASTNode & callingNode,
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
                                                       AST::ASTNode & callingNode,
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
                                                       AST::ASTNode & callingNode,
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
                                                     AST::ASTNode & callingNode,
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
                                                     AST::ASTNode & callingNode,
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
                                                         AST::ASTNode & callingNode,
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

llvm::Constant * GenericSignedIntDataType::codegenConstCmpLTOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericSignedIntDataType::codegenConstCmpLEOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericSignedIntDataType::codegenConstCmpGTOp(AST::ASTNode & callingNode,
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

llvm::Constant * GenericSignedIntDataType::codegenConstCmpGEOp(AST::ASTNode & callingNode,
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
