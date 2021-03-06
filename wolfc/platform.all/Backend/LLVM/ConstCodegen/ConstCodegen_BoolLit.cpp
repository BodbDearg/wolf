//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/BoolLit.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::BoolLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the data type for the bool
    {
        const DataType & dataType = PrimitiveDataTypes::getBoolDataType();
        WC_ASSERT(dataType.isBool());
        dataType.accept(mCodegenDataType);
    }
    
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    
    // Create the constant and save to the stack
    llvm::Constant * constant = astNode.mToken.type == TokenType::kTrue ?
        mCtx.mIRBuilder.getTrue() :
        mCtx.mIRBuilder.getFalse();
    
    mCtx.pushConstant(Constant(constant, compiledType, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
