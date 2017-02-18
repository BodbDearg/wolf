#include "AddrCodegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/ArrayLit.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void AddrCodegen::visit(const AST::ArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the literal
    astNode.accept(mCodegen);
    Value exprVal = mCtx.popValue();
    
    // The value must be valid for this to work
    WC_GUARD(exprVal.isValid());
    
    // Create an alloca to hold the literal on the stack so we can take it's address
    const CompiledDataType & exprValCDT = exprVal.mCompiledType;
    llvm::Value * llvmStackVal = mCtx.mIRBuilder.CreateAlloca(exprValCDT.getLLVMType(),
                                                              nullptr,
                                                              "AddrCodegen:ArrayLit:Alloca");
    
    WC_ASSERT(llvmStackVal);
    mCtx.mIRBuilder.CreateStore(exprVal.mLLVMVal, llvmStackVal);
    
    // Push it onto the codegen context stack
    mCtx.pushValue(Value(llvmStackVal, exprValCDT, true, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
