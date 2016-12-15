#include "WCUnknownDataType.hpp"

#include "AST/WCPrintStmnt.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "<?!unknown!?>";

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

llvm::AllocaInst * UnknownDataType::codegenAlloca(CodegenCtx & cgCtx,
                                                  ASTNode & callingNode,
                                                  const std::string & instLabel)
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(instLabel);
    callingNode.compileError("Can't allocate type '%s' on the stack!", name().c_str());
    return nullptr;
}

bool UnknownDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                        const PrintStmnt & parentPrintStmnt,
                                        llvm::Constant & printfFn,
                                        llvm::Value & value) const
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(value);
    parentPrintStmnt.compileError("Unknown datatype cannot be printed!");
    return false;
}

bool UnknownDataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    // No codegen to do for the unknown data type:
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(callingNode);
    callingNode.compileError("Cannot generate an llvm data type for the 'unknown' data type!", name().c_str());
    return true;
}

WC_END_NAMESPACE
