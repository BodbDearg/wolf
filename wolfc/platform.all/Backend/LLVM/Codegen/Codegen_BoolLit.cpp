#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/BoolLit.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/BoolDataType.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the int
    const BoolDataType & dataType = PrimitiveDataTypes::getBoolDataType();
    dataType.accept(mCodegenDataType);
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Create the value and save to the stack
    WC_ASSERT(compiledType.isValid());
    llvm::Value * llvmValue = astNode.mToken.type == TokenType::kTrue ?
        mCtx.mIRBuilder.getTrue() :
        mCtx.mIRBuilder.getFalse();
    
    mCtx.pushValue(Value(llvmValue, compiledType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE