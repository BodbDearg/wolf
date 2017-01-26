#include "ConstCodegen.hpp"

#include "AST/Nodes/WCIntLit.hpp"
#include "CodegenCtx.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::IntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // TODO: data type should be based on the numeric literal and precision suffixes used
    llvm::Type * llvmType = llvm::Type::getInt64Ty(mCtx.mLLVMCtx);
    llvm::Constant * constant = llvm::ConstantInt::get(llvmType, astNode.mToken.data.intVal);
    WC_ASSERT(constant);
    mCtx.pushLLVMConstant(*constant);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
