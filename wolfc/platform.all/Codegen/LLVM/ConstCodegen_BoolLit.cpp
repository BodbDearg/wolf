#include "ConstCodegen.hpp"

#include "AST/Nodes/WCBoolLit.hpp"
#include "CodegenCtx.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Create the constant
    llvm::Constant * constant = astNode.mToken.type == TokenType::kTrue ?
        mCtx.mIRBuilder.getTrue() :
        mCtx.mIRBuilder.getFalse();
    
    // Save the constant created
    WC_ASSERT(constant);
    mCtx.pushLLVMConstant(*constant);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
