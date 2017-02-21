#include "ConstCodegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/IntLit.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void ConstCodegen::visit(const AST::IntLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Get the token for the integer:
    const Token & tok = astNode.mToken;
    WC_ASSERT(tok.type == TokenType::kIntLit);
    
    // Codegen the data type for the int
    {
        const DataType & dataType = PrimitiveDataTypes::getUsingTypeId(tok.data.intData.dataTypeId);
        
        if (dataType.isInteger()) {
            dataType.accept(mCodegenDataType);
        }
        else {
            mCtx.error(astNode,
                       "Can't codegen integer literal because it's of an invalid data type '%s'!",
                       dataType.name().c_str());
            
            return;
        }
    }
    
    CompiledDataType compiledType = mCtx.popCompiledDataType();
    WC_ASSERT(compiledType.isValid());
    
    // Figure out how many bits are needed to hold the integer and if we have enough
    const auto & tokIntData = tok.data.intData;
    size_t dataTypeSizeInBits = compiledType.getLLVMType()->getScalarSizeInBits();
    size_t bitsNeededForInt = llvm::APInt::getBitsNeeded(tokIntData.strMinusUS, uint8_t(tokIntData.base));
    
    if (bitsNeededForInt > dataTypeSizeInBits) {
        mCtx.error(astNode,
                   "Integer literal cannot fit into target data type '%s'! Target data type is %zu bits "
                   "but the integer literal requires %zu bits to store!",
                   compiledType.getDataType().name().c_str(),
                   dataTypeSizeInBits,
                   bitsNeededForInt);
        
        return;
    }
    
    // Now get the arbitrary precision integer
    llvm::APInt apInt(unsigned(dataTypeSizeInBits), tokIntData.strMinusUS, uint8_t(tokIntData.base));
    
    if (compiledType.getLLVMType()->getScalarSizeInBits() < apInt.getBitWidth()) {
        mCtx.error(astNode,
                   "Integer literal cannot fit into target data type '%s'! Target data type is %zu bits "
                   "but the integer literal requires %zu bits to store!",
                   size_t(compiledType.getLLVMType()->getScalarSizeInBits()),
                   apInt.getBitWidth());
    }
    
    // Create the constant and save to the stack
    WC_ASSERT(compiledType.getLLVMType());
    llvm::Constant * constant = llvm::ConstantInt::get(compiledType.getLLVMType(), apInt);
    WC_ASSERT(constant);
    mCtx.pushConstant(Constant(constant, compiledType, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
