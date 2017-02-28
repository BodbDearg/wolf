#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/IntLit.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"
#include "Lexer/Token.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::IntLit & astNode) {
    // This just uses the constant code generator, it's the same thing:
    astNode.accept(mConstCodegen);
    
    // Rebrand the result as a non constant however,
    Constant intConstant = mCtx.popConstant();
    WC_GUARD(intConstant.isValid());
    mCtx.pushValue(Value(intConstant.mLLVMConst, intConstant.mCompiledType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
