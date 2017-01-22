#include "WCBoolDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCPrintStmnt.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "bool";

void BoolDataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId BoolDataType::getTypeId() const {
    return DataTypeId::kBool;
}

const std::string & BoolDataType::name() const {
    return kName;
}

bool BoolDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const BoolDataType*>(&other) != nullptr;
}

#warning FIXME - Codegen
#if 0
llvm::Value * BoolDataType::codegenCmpEQOp(CodegenCtx & cgCtx,
                                           AST::ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpEQ,
                                            kOpName_CmpEQ,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpEQ(&leftVal, &rightVal, "Bool_CmpEQOp");
}

llvm::Value * BoolDataType::codegenCmpNEOp(CodegenCtx & cgCtx,
                                           AST::ASTNode & callingNode,
                                           llvm::Value & leftVal,
                                           DataType & rightTy,
                                           llvm::Value & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpNE,
                                            kOpName_CmpNE,
                                            rightTy), nullptr);
    
    return cgCtx.irBuilder.CreateICmpNE(&leftVal, &rightVal, "Bool_CmpNEOp");
}

llvm::Constant * BoolDataType::codegenConstCmpEQOp(AST::ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpEQ,
                                            kOpName_CmpEQ,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_EQ, &leftVal, &rightVal);
}

llvm::Constant * BoolDataType::codegenConstCmpNEOp(AST::ASTNode & callingNode,
                                                   llvm::Constant & leftVal,
                                                   DataType & rightTy,
                                                   llvm::Constant & rightVal)
{
    WC_GUARD(compileCheckBinaryOpTypesMatch(callingNode,
                                            kOpSymbol_CmpNE,
                                            kOpName_CmpNE,
                                            rightTy), nullptr);
    
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_NE, &leftVal, &rightVal);
}
#endif

WC_END_NAMESPACE
