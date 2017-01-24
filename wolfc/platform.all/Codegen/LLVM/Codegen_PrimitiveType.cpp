#include "Codegen.hpp"

#include "AST/Nodes/WCPrimitiveType.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::PrimitiveType & astNode) {
    // This is a constant operation, delegate this to the constant code generator.
    // Note, not making a note of the current node visited because the constant
    // code generator will do that already:
    astNode.accept(mConstCodegen);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
