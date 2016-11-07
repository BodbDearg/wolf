#include "WCDataType.hpp"

#include "WCASTNode.hpp"

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

bool DataType::isUnknown() const {
    return false;   // False for all types except 'Unknown'
}

bool DataType::isVoid() const {
    return false;   // False for all types except 'void'
}

bool DataType::isBool() const {
    return false;   // False for all types except 'bool'
}

bool DataType::isArray() const {
    return false;   // False for all types except 'array'
}

bool DataType::codegenLLVMTypeIfRequired(CodegenCtx & cgCtx, ASTNode & callingNode) {
    if (mLLVMType) {
        return true;
    }
    
    return codegenLLVMType(cgCtx, callingNode);
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
