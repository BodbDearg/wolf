#include "WCVoidDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "AST/Nodes/WCPrintStmnt.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

static const std::string kName = "void";

DataTypeId VoidDataType::getTypeId() const {
    return DataTypeId::kVoid;
}

const std::string & VoidDataType::name() const {
    return kName;
}

bool VoidDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const VoidDataType*>(&other) != nullptr;
}

bool VoidDataType::isSized() const {
    return false;
}

#warning FIXME - Codegen
#if 0
llvm::AllocaInst * VoidDataType::codegenAlloca(CodegenCtx & cgCtx,
                                               AST::ASTNode & callingNode,
                                               const std::string & instLabel)
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(instLabel);
    callingNode.compileError("Can't allocate type '%s' on the stack!", name().c_str());
    return nullptr;
}

bool VoidDataType::codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) {
    mLLVMType = llvm::Type::getVoidTy(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}
#endif

WC_END_NAMESPACE
