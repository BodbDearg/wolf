#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/FuncCall.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::FuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Generate the basic argument values firstly
    const auto & args = astNode.getArgs();
    std::vector<Value> argVals;
    argVals.reserve(args.size());
    
    for (const AST::AssignExpr * assignExpr : args) {
        WC_ASSERT(assignExpr);
        assignExpr->accept(*this);
        argVals.push_back(mCtx.popValue());
    }
    
    // Now push the values into the codegen context, but for each argument that requires storage such as
    // an Array, Struct etc., allocate room for a copy of it on the stack and pass a pointer to this copy
    // instead. This is how languages like C handle the passing of structs.
    for (const Value & argVal : argVals) {
        // If the arg is not valid just push it back on the stack verbatim
        if (!argVal.isValid()) {
            mCtx.pushValue(argVal);
            continue;
        }
        
        // Okay, see if we need to modify this:
        const CompiledDataType & argCDT = argVal.mCompiledType;
        const DataType & argDT = argCDT.getDataType();
        
        if (argDT.requiresStorage()) {
            // Value requires stack storage: need to alloc stack storage for it:
            llvm::Value * argStackValue = mCtx.mIRBuilder.CreateAlloca(argCDT.getLLVMType());
            WC_ASSERT(argStackValue);
            
            // Store the value in this slot:
            llvm::Value * storeInst = mCtx.mIRBuilder.CreateStore(argVal.mLLVMVal, argStackValue);
            WC_ASSERT(storeInst);
            
            // The arg value stack pointer is what gets passed into the function:
            mCtx.pushValue(Value(argStackValue, argCDT, true, argVal.mDeclaringNode));
        }
        else {
            // Arg is not an Array, Struct etc. type which requires storage.
            // Push this value onto the stack unmodified:
            mCtx.pushValue(argVal);
        }
    }
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
