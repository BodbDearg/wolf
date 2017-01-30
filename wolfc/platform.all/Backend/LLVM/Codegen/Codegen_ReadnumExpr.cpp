#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/ReadnumExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/DataTypeId.hpp"
#include "DataType/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::ReadnumExpr & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Get 'scanf' C function
    llvm::Constant * scanfFn = mCtx.mLLVMModule->getFunction("scanf");
    
    if (!scanfFn) {
        mCtx.error(astNode, "Codegen of readnum() expression failed! Can't find 'scanf()' function!");
        return;
    }
    
    // Get 'getchar' C function
    llvm::Constant * getcharFn = mCtx.mLLVMModule->getFunction("getchar");
    
    if (!getcharFn) {
        mCtx.error(astNode, "Codegen of readnum() expression failed! Can't find 'getchar()' function!");
        return;
    }
    
    // Create a format string for scanf
    llvm::Value * fmtStr = mCtx.mIRBuilder.CreateGlobalStringPtr("%zd", "ReadnumExpr:fmt_str");
    WC_ASSERT(fmtStr);
    
    // Create a stack var to hold the output:
    llvm::Value * outputVar = mCtx.mIRBuilder.CreateAlloca(mCtx.mIRBuilder.getInt64Ty(),
                                                           nullptr,
                                                           "ReadnumExpr:tmp_stack_var");
    
    WC_ASSERT(outputVar);
    
    // Create the call to scanf!
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCall(scanfFn,
                                              { fmtStr, outputVar },
                                              "ReadnumExpr:scanf_call"));
    
    // Consume the following return character
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateCall(getcharFn,
                                              {},
                                              "ReadnumExpr:getchar_call"));
    
    // Get the data type for the return:
    {
        const DataType & int64DataType = PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64);
        int64DataType.accept(mCodegenDataType);
    }
    
    CompiledDataType returnType = mCtx.popCompiledDataType();
    
    // Save the result:
    llvm::Value * returnVal = mCtx.mIRBuilder.CreateLoad(outputVar, "ReadnumExpr:load_tmp_stack_var");
    WC_ASSERT(returnVal);
    mCtx.pushValue(Value(returnVal, returnType, false, &astNode));
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
