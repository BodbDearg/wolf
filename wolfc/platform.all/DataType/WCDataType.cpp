#include "WCDataType.hpp"

#include "AST/WCASTNode.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

const char * const DataType::kOpSymbol_Add = "+";
const char * const DataType::kOpSymbol_Sub = "-";
const char * const DataType::kOpSymbol_BOr = "|";
const char * const DataType::kOpSymbol_BXOr = "^";

const char * const DataType::kOpName_Add = "add";
const char * const DataType::kOpName_Sub = "subtract";
const char * const DataType::kOpName_BOr = "bitwise or";
const char * const DataType::kOpName_BXOr = "bitwise xor";

DataType::~DataType() {
    WC_EMPTY_FUNC_BODY();
}

bool DataType::isValid() const {
    return true;    // True for all types except 'Unknown'
}

bool DataType::isSized() const {
    return true;    // True for all types except 'Unknown' and 'Void'
}

bool DataType::isInteger() const {
    return false;   // False by default for everything except integer types
}

bool DataType::requiresStorage() const {
    return false;   // False by default for all types
}

bool DataType::codegenLLVMTypeIfRequired(CodegenCtx & cgCtx, ASTNode & callingNode) {
    if (mLLVMType) {
        return true;
    }
    
    return codegenLLVMType(cgCtx, callingNode);
}

llvm::AllocaInst * DataType::codegenAlloca(CodegenCtx & cgCtx,
                                           ASTNode & callingNode,
                                           const std::string & instLabel)
{
    WC_GUARD_ASSERT(mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
}

bool DataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                 ASTNode & callingNode,
                                 llvm::Constant & printfFn,
                                 llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(valToPrint);
    callingNode.compileError("print() is not supported for expression of type '%s'!", name().c_str());
    return false;
}

llvm::Value * DataType::codegenCastTo(CodegenCtx & cgCtx,
                                      ASTNode & callingNode,
                                      llvm::Value & valToCast,
                                      DataType & castToTy)
{
    // If the type to cast to is the same as this type then do nothing, cast not needed:
    if (equals(castToTy)) {
        return &valToCast;
    }
    
    // The default impl of this just raises an error
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(valToCast);
    
    callingNode.compileError("Cast from type left type '%s' to right type '%s' is not allowed!",
                             name().c_str(),
                             castToTy.name().c_str());
    
    return nullptr;
}

llvm::Value * DataType::codegenAddOp(CodegenCtx & cgCtx,
                                     ASTNode & callingNode,
                                     llvm::Value & leftVal,
                                     DataType & rightTy,
                                     llvm::Value & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_Add, kOpName_Add, rightTy);
    return nullptr;
}

llvm::Value * DataType::codegenSubOp(CodegenCtx & cgCtx,
                                     ASTNode & callingNode,
                                     llvm::Value & leftVal,
                                     DataType & rightTy,
                                     llvm::Value & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_Sub, kOpName_Sub, rightTy);
    return nullptr;
}

llvm::Value * DataType::codegenBOrOp(CodegenCtx & cgCtx,
                                     ASTNode & callingNode,
                                     llvm::Value & leftVal,
                                     DataType & rightTy,
                                     llvm::Value & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_BOr, kOpName_BOr, rightTy);
    return nullptr;
}

llvm::Value * DataType::codegenBXOrOp(CodegenCtx & cgCtx,
                                      ASTNode & callingNode,
                                      llvm::Value & leftVal,
                                      DataType & rightTy,
                                      llvm::Value & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_BXOr, kOpName_BXOr, rightTy);
    return nullptr;
}

llvm::Constant * DataType::codegenConstAddOp(ASTNode & callingNode,
                                             llvm::Constant & leftVal,
                                             DataType & rightTy,
                                             llvm::Constant & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_Add, kOpName_Add, rightTy);
    return nullptr;
}

llvm::Constant * DataType::codegenConstSubOp(ASTNode & callingNode,
                                             llvm::Constant & leftVal,
                                             DataType & rightTy,
                                             llvm::Constant & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_Sub, kOpName_Sub, rightTy);
    return nullptr;
}

llvm::Constant * DataType::codegenConstBOrOp(ASTNode & callingNode,
                                             llvm::Constant & leftVal,
                                             DataType & rightTy,
                                             llvm::Constant & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_BOr, kOpName_BOr, rightTy);
    return nullptr;
}

llvm::Constant * DataType::codegenConstBXOrOp(ASTNode & callingNode,
                                              llvm::Constant & leftVal,
                                              DataType & rightTy,
                                              llvm::Constant & rightVal)
{
    // The default impl simply issues an error that the operator is not available
    WC_UNUSED_PARAM(leftVal);
    WC_UNUSED_PARAM(rightVal);
    issueOpNotAvailableCompileError(callingNode, kOpSymbol_BXOr, kOpName_BXOr, rightTy);
    return nullptr;
}

bool DataType::compileCheckLLVMTypeDefined(ASTNode & callingNode) {
    WC_GUARD(!mLLVMType, true);
    callingNode.compileError("The LLVM type for data type '%s' is undefined!", name().c_str());
    return false;
}

bool DataType::compileCheckBinaryOpTypesMatch(ASTNode & callingNode,
                                              const char * opSymbol,
                                              const char * opName,
                                              DataType & rightExprType)
{
    // If the types match then all is ok
    if (equals(rightExprType)) {
        return true;
    }
    
    // Issue the compile error:
    callingNode.compileError("The types on both sides of the '%s' (%s) operator are incompatible! "
                             "The left expression type '%s' does not match the right expression type '%s'!",
                             opSymbol,
                             opName,
                             name().c_str(),
                             rightExprType.name().c_str());
    
    // Check failed:
    return false;
}

void DataType::issueOpNotAvailableCompileError(ASTNode & callingNode,
                                               const char * opSymbol,
                                               const char * opName,
                                               DataType & rightExprType) const
{
    callingNode.compileError("The '%s' (%s) operator is not supported for a left "
                             "expression of type '%s' with a right expression of type '%s'!",
                             opSymbol,
                             opName,
                             name().c_str(),
                             rightExprType.name().c_str());
}

void DataType::issueGenericCodegenLLVMTypeError(ASTNode & callingNode) const {
    callingNode.compileError("Failed to codegen the llvm type for data type '%s'!", name().c_str());
}

WC_END_NAMESPACE
