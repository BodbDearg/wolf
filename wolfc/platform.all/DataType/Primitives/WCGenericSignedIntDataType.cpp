#include "WCGenericSignedIntDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"

WC_BEGIN_NAMESPACE

#warning FIXME - Codegen
#if 0
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
