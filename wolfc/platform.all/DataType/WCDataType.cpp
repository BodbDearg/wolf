#include "WCDataType.hpp"

#include "AST/WCASTNode.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

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

llvm::Value * DataType::codegenCastTo(CodegenCtx & cgCtx,
                                      ASTNode & callingNode,
                                      llvm::Value & valueToCast,
                                      DataType & castToType)
{
    // If the type to cast to is the same as this type then do nothing, cast not needed:
    if (equals(castToType)) {
        return &valueToCast;
    }
    
    // The default impl of this just raises an error
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(valueToCast);
    
    callingNode.compileError("Cast from type '%s' to '%s' is not allowed!",
                             name().c_str(),
                             castToType.name().c_str());
    
    return nullptr;
}

bool DataType::compileCheckLLVMTypeDefined(ASTNode & callingNode) {
    WC_GUARD(!mLLVMType, true);
    callingNode.compileError("The LLVM type for data type '%s' is undefined!", name().c_str());
    return false;
}

void DataType::issueGenericCodegenLLVMTypeError(ASTNode & callingNode) {
    callingNode.compileError("Failed to codegen the llvm type for data type '%s'!", name().c_str());
}

WC_END_NAMESPACE
