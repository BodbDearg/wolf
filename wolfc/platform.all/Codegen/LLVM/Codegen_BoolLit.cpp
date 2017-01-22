#include "Codegen.hpp"

#include "AST/Nodes/WCBoolLit.hpp"
#include "Lexer/WCToken.hpp"
#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    llvm::Value * value = mCtx.mIRBuilder.getInt1(astNode.mToken.type == TokenType::kTrue);
    WC_ASSERT(value);
    mCtx.pushLLVMValue(*value);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
