#include "WCUnknownDataType.hpp"

#include "AST/Nodes/WCPrintStmnt.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "<Unknown>";

DataTypeId UnknownDataType::getTypeId() const {
    return DataTypeId::kUnknown;
}

const std::string & UnknownDataType::name() const {
    return kName;
}

bool UnknownDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const UnknownDataType*>(&other) != nullptr;
}

bool UnknownDataType::isValid() const {
    return false;   // The unknown data type is the only type which is not valid
}

bool UnknownDataType::isSized() const {
    return false;
}

#warning FIXME - Codegen
#if 0
llvm::AllocaInst * UnknownDataType::codegenAlloca(CodegenCtx & cgCtx,
                                                  AST::ASTNode & callingNode,
                                                  const std::string & instLabel)
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(instLabel);
    callingNode.compileError("Can't allocate type '%s' on the stack!", name().c_str());
    return nullptr;
}

bool UnknownDataType::codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) {
    // No codegen to do for the unknown data type:
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(callingNode);
    callingNode.compileError("Cannot generate an llvm data type for the 'unknown' data type!", name().c_str());
    return true;
}
#endif

WC_END_NAMESPACE
