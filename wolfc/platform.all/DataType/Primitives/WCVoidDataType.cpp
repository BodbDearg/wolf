#include "WCVoidDataType.hpp"
#include "WCASTNode.hpp"
#include "WCCodegenCtx.hpp"
#include "WCPrintStmnt.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

static const std::string kName = "void";

const std::string & VoidDataType::name() const {
    return kName;
}

bool VoidDataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const VoidDataType*>(&other) != nullptr;
}

bool VoidDataType::isSized() const {
    return false;
}

bool VoidDataType::isVoid() const {
    return true;
}

bool VoidDataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    mLLVMType = llvm::Type::getVoidTy(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        callingNode.compileError("Failed to generate llvm type for data type '%s'!",
                                 name().c_str());
        
        return false;
    }
    
    return true;
}

bool VoidDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                     const PrintStmnt & parentPrintStmnt,
                                     llvm::Constant & printfFn,
                                     llvm::Value & value) const
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(value);
    parentPrintStmnt.compileError("Type 'void' cannot be printed!");
    return false;
}

WC_END_NAMESPACE
