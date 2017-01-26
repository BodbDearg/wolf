#include "Codegen.hpp"

#include "AST/Nodes/WCAssignExpr.hpp"
#include "AST/Nodes/WCIdentifier.hpp"
#include "AST/Nodes/WCType.hpp"
#include "AST/Nodes/WCVarDecl.hpp"
#include "Codegen.hpp"
#include "CodegenCtx.hpp"
#include "DataType/WCDataType.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

/*
bool VarDecl::codegenAsGlobalVar(CodegenCtx & cgCtx) { 
    // Now evaluate the right expression:
    llvm::Constant * rightValue = mInitExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(rightValue, false);
    
    // Create the variable. If this fails then the variable already exists:
    DataType & varDataType = dataType();
    
    // Data type for the var must equal the data type of the expression:
    DataType & exprDataType = mInitExpr.dataType();
    
    if (!varDataType.equals(exprDataType)) {
        // TODO: Handle auto type promotion here
        compileError("Initializing expression for variable declaration must be of type '%s', not '%s'!",
                     varDataType.name().c_str(),
                     exprDataType.name().c_str());
        
        return false;
    }
    
    // Create the value
    DataValue * leftValue = cgCtx.module.createVar(mIdent.mToken.data.strVal.ptr,
                                                   varDataType,
                                                   rightValue,
                                                   cgCtx,
                                                   *this);
    
    if (!leftValue) {
        compileError("The global variable '%s' has been redefined!", mIdent.mToken.data.strVal.ptr);
        return false;
    }
    
    return true;    // All good!
}
*/

static void codegenLocalVarDeclWithType(Codegen & cg,
                                        const AST::VarDecl & varDecl,
                                        const CompiledDataType & varCompiledType,
                                        bool varDataTypeInferred)
{
    #warning TODO: local var codegen
}

static void codegenGlobalVarDeclWithType(Codegen & cg,
                                         const AST::VarDecl & varDecl,
                                         const CompiledDataType & varCompiledType,
                                         bool varDataTypeInferred)
{
    // Evaluate the initializer expression as a constant
    varDecl.mInitExpr.accept(cg.mConstCodegen);
    llvm::Constant * varInitLLVMVal = cg.mCtx.popLLVMConstant();
    
    // Ensure the initializer is the same type as the var decl if the var type is not inferred.
    // If we are not using inference there might be a mismatch...
    const DataType & varType = varCompiledType.getDataType();
    bool initializerTypeIsOk = true;
    
    if (!varDataTypeInferred) {
        // Compile the type of the variable initializer
        varDecl.mInitExpr.dataType().accept(cg.mConstCodegen.mCodegenDataType);
        CompiledDataType varInitCompiledType = cg.mCtx.popCompiledDataType();
        const DataType & varInitType = varInitCompiledType.getDataType();
        
        // Make sure the initializer and variable types match
        if (!varInitType.equals(varType)) {
            cg.mCtx.error(varDecl,
                          "Variable initializer type mismatch! Variable is of type '%s' and "
                          "initializer is of type '%s'!",
                          varType.name().c_str(),
                          varInitType.name().c_str());
            
            initializerTypeIsOk = false;
        }
    }
    
    // Ensure the variable data type is ok
    bool varTypeIsOk = true;
    
    if (!varType.isValid()) {
        cg.mCtx.error(varDecl, "Can't declare a variable of invalid type '%s'!", varType.name().c_str());
        varTypeIsOk = false;
    }
    else if (!varType.isSized()) {
        cg.mCtx.error(varDecl, "Can't declare a variable of type '%s' which has no size!", varType.name().c_str());
        varTypeIsOk = false;
    }
    
    // Alright, lets allocate a new global
    if (varTypeIsOk) {
        // If the initializer is bad then codegen without it
        if (!initializerTypeIsOk) {
            varInitLLVMVal = nullptr;
        }
        
        // TODO: use the linear allocator here
        const char * varName = varDecl.mIdent.name();
        auto * varLLVMVar = new llvm::GlobalVariable(*cg.mCtx.mLLVMModule.get(),
                                                     varCompiledType.getLLVMType(),
                                                     false,                               // Not constant
                                                     llvm::GlobalValue::PrivateLinkage,
                                                     varInitLLVMVal,
                                                     varName);
        
        WC_ASSERT(varLLVMVar);
        
        // Register the variable. If it's registered more than once then this will generate an error.
        cg.mCtx.mModuleVarContainer.createVar(cg.mCtx,
                                              varName,
                                              varType,
                                              *varLLVMVar,
                                              true,
                                              varDecl);
    }
}

static void codegenVarDeclWithType(Codegen & cg,
                                   const AST::VarDecl & varDecl,
                                   const CompiledDataType & varCompiledType,
                                   bool varDataTypeInferred)
{
    // Codegen as either a local or global variable definition
    if (cg.mCtx.mCurFunction) {
        codegenLocalVarDeclWithType(cg,
                                    varDecl,
                                    varCompiledType,
                                    varDataTypeInferred);
    }
    else {
        codegenGlobalVarDeclWithType(cg,
                                     varDecl,
                                     varCompiledType, 
                                     varDataTypeInferred);
    }
}

void Codegen::visit(const AST::VarDeclExplicitType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the explict data type:
    astNode.mType.accept(*this);
    CompiledDataType varCompiledType = mCtx.popCompiledDataType();
    codegenVarDeclWithType(*this, astNode, varCompiledType, false);
}

void Codegen::visit(const AST::VarDeclInferType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the inferred data type:
    astNode.mInitExpr.dataType().accept(mConstCodegen.mCodegenDataType);
    CompiledDataType varCompiledType = mCtx.popCompiledDataType();
    codegenVarDeclWithType(*this, astNode, varCompiledType, true);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
