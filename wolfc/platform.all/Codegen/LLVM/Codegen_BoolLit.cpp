#include "Codegen.hpp"

#include "AST/Nodes/WCBoolLit.hpp"
#include "CodegenCtx.hpp"
#include "DataType/Primitives/WCBoolDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void Codegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the int
    const BoolDataType & dataType = PrimitiveDataTypes::getBoolDataType();
    dataType.accept(mConstCodegen.mCodegenDataType);
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Create the value and save to the stack
    WC_ASSERT(compiledType.isValid());
    llvm::Value * llvmValue = astNode.mToken.type == TokenType::kTrue ?
        mCtx.mIRBuilder.getTrue() :
        mCtx.mIRBuilder.getFalse();
    
    mCtx.pushValue(Value(llvmValue, compiledType, false, &astNode));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
