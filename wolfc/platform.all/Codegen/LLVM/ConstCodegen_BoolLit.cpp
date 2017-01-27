#include "ConstCodegen.hpp"

#include "AST/Nodes/WCBoolLit.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "DataType/Primitives/WCBoolDataType.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the bool
    const DataType & dataType = PrimitiveDataTypes::getBoolDataType();
    WC_ASSERT(dataType.isBool());
    dataType.accept(mCodegenDataType);
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Create the constant and save to the stack
    llvm::Constant * constant = astNode.mToken.type == TokenType::kTrue ?
        mCtx.mIRBuilder.getTrue() :
        mCtx.mIRBuilder.getFalse();
    
    mCtx.pushConstant(Constant(constant, compiledType, &astNode));
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
