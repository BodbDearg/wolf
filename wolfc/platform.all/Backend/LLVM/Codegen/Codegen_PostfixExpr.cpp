#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/FuncCall.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/Primitives/FuncDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

void Codegen::visit(const AST::PostfixExprNoPostfix & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    astNode.mExpr.accept(*this);
}

void Codegen::visit(const AST::PostfixExprInc & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprDec & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

void Codegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // First of all lookup the expression we perform the call on
    astNode.mExpr.accept(*this);
    Value operandVal = mCtx.popValue();
    
    // Determine if the function to call is valid
    const CompiledDataType & operandCompiledType = operandVal.mCompiledType;
    const DataType & operandDataType = operandCompiledType.getDataType();
    const FuncDataType * funcDataType = dynamic_cast<const FuncDataType*>(&operandDataType);
    bool isOperandValid = true;
    
    if (!operandDataType.isFunc()) {
        isOperandValid = false;
        mCtx.error(astNode.mExpr,
                   "Operand of a function call must be of a function type, not type '%s'!",
                   operandDataType.name().c_str());
    }
    else if (!operandCompiledType.getLLVMType()) {
        isOperandValid = false;
        mCtx.error(astNode.mExpr,
                   "Can't generate function call to function of type '%s' because we do not have a "
                   "valid llvm type generated for this function!",
                   operandDataType.name().c_str());
    }
    else if (!funcDataType) {
        isOperandValid = false;
        mCtx.error(astNode.mExpr,
                   "Internal compiler error! Can't generate function call to function of type '%s' "
                   "because it's type has not been evaluated yet!",
                   operandDataType.name().c_str());
    }
    
    // Evalute the values for the function args
    astNode.mFuncCall.accept(*this);
    
    // Now grab the arg values and see if they are all ok
    size_t numArgs = astNode.mFuncCall.getArgs().size();
    std::vector<Value> funcArgVals;
    funcArgVals.reserve(numArgs);
    bool funcArgsOk = true;
    
    for (size_t i = 0; i < numArgs; ++i) {
        // Grab the arg value from the stack
        funcArgVals.push_back(mCtx.popValue());
        const Value & arg = funcArgVals.back();
        
        // Make sure the arg is ok
        if (!arg.isValid()) {
            funcArgsOk = false;
        }
    }
    
    // Make sure the number of args matches the number of args expected to the function.
    // If the arg counts match, make sure arg types agree.
    if (funcDataType) {
        if (funcArgVals.size() == funcDataType->mArgTypes.size()) {
            for (size_t i = 0; i < funcArgVals.size(); ++i) {
                // Get the type of the arg expression and type of the arg in the function signature
                const DataType & argExprDT = funcArgVals[i].mCompiledType.getDataType();
                const DataType & funcArgDT = *funcDataType->mArgTypes[i];
                
                // TODO: support auto promotion
                // Make sure the arg types match:
                if (!argExprDT.equals(funcArgDT)) {
                    mCtx.error(funcArgVals[i].mDeclaringNode ? *funcArgVals[i].mDeclaringNode : astNode,
                               "Argument type mismatch for arg #%zu in function call! Expected type '%s' "
                               "for the argument, instead got type '%s'!",
                               i + 1,
                               funcArgDT.name().c_str(),
                               argExprDT.name().c_str());
                    
                    funcArgsOk = false;
                }
            }
        }
        else {
            mCtx.error(astNode,
                       "Invalid number of arguments to function called! Expected '%zu', got '%zu' instead!",
                       funcDataType->mArgTypes.size(),
                       funcArgVals.size());
            
            funcArgsOk = false;
        }
    }
    else {
        funcArgsOk = false;
    }
    
    // Okay, if everything is good then generate the function call:
    if (isOperandValid && funcArgsOk) {
        // Generate a vector of the llvm arguments
        std::vector<llvm::Value*> llvmArgs;
        llvmArgs.reserve(funcArgVals.size());
        
        for (const Value & value : funcArgVals) {
            WC_ASSERT(value.mLLVMVal);
            llvmArgs.push_back(value.mLLVMVal);
        }
        
        // Get the llvm function
        llvm::Function * llvmFunc = llvm::cast<llvm::Function>(operandVal.mLLVMVal);
        
        // Call it: note if non void we have to give the value a name
        llvm::Value * callResult = nullptr;
        
        if (funcDataType->mReturnType.isVoid()) {
            callResult = mCtx.mIRBuilder.CreateCall(llvmFunc,
                                                    llvmArgs);
        }
        else {
            callResult = mCtx.mIRBuilder.CreateCall(llvmFunc,
                                                    llvmArgs,
                                                    "PostfixExprFuncCall:ReturnVal");
        }
        
        // Get the compiled data type for the return type
        funcDataType->mReturnType.accept(mCodegenDataType);
        CompiledDataType returnCompiledType = mCtx.popCompiledDataType();
        
        // Save the result of the call:
        if (!funcDataType->mReturnType.isVoid()) {
            mCtx.pushValue(Value(callResult, returnCompiledType, false, &astNode));
        }
    }
}

void Codegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    #warning TODO: Codegen this node
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
