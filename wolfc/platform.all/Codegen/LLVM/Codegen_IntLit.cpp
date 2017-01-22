#include "Codegen.hpp"

#include "AST/Nodes/WCIntLit.hpp"
#include "Lexer/WCToken.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::IntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // TODO: data type should be based on the numeric literal and precision suffixes used
    llvm::Value * value = mCtx.mIRBuilder.getInt64(astNode.mToken.data.intVal);
    WC_ASSERT(value);
    mCtx.pushLLVMValue(*value);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
