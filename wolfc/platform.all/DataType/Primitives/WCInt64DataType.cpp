#include "WCInt64DataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "DataType/DataTypeVisitor.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

static const std::string kName = "int64";

void Int64DataType::accept(DataTypeVisitor & visitor) const {
    visitor.visit(*this);
}

DataTypeId Int64DataType::getTypeId() const {
    return DataTypeId::kInt64;
}

const std::string & Int64DataType::name() const {
    return kName;
}

bool Int64DataType::equals(const DataType & other) const {
    return this == &other || dynamic_cast<const Int64DataType*>(&other) != nullptr;
}

#warning FIXME - Codegen
#if 0
bool Int64DataType::codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) {
    // Lazy out if already done
    WC_GUARD(!mLLVMType, true);
    mLLVMType = llvm::Type::getInt64Ty(cgCtx.llvmCtx);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

bool Int64DataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                      AST::ASTNode & callingNode,
                                      llvm::Constant & printfFn,
                                      llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(callingNode);
    
    // Create a format string for printf and call
    llvm::Value * fmtStr = cgCtx.irBuilder.CreateGlobalStringPtr("%lld", "print_fmt_str:int");
    return cgCtx.irBuilder.CreateCall(&printfFn,
                                      { fmtStr, &valToPrint },
                                      "print_printf_call:int") != nullptr;
}
#endif

WC_END_NAMESPACE
