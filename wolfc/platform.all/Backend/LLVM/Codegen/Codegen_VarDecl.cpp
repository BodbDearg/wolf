#include "Codegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/Identifier.hpp"
#include "AST/Nodes/Scope.hpp"
#include "AST/Nodes/Type.hpp"
#include "AST/Nodes/VarDecl.hpp"
#include "DataType/DataType.hpp"
#include "StringUtils.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* Prefix for all var labels */
static const std::string kVarLabelPrefix = "alloc_var:";

/**
 * Check that the initializer is the right type and check that the variable declaration type
 * is valid and nothing something weird like 'void'.
 */
static void doVarDeclTypeChecks(Codegen & cg,
                                const AST::VarDecl & varDecl,
                                const CompiledDataType & varCompiledType,
                                bool varTypeIsInferred,
                                bool & initializerTypeIsOkOut,
                                bool & varTypeIsOkOut)
{
    // Ensure the initializer is the same type as the var decl if the var type is not inferred.
    // If we are not using inference there might be a mismatch...
    const DataType & varType = varCompiledType.getDataType();
    initializerTypeIsOkOut = true;
    
    if (!varTypeIsInferred) {
        // Compile the type of the variable initializer
        varDecl.mInitExpr.dataType().accept(cg.mCodegenDataType);
        CompiledDataType varInitCompiledType = cg.mCtx.popCompiledDataType();
        const DataType & varInitType = varInitCompiledType.getDataType();
        
        // Make sure the initializer and variable types match
        if (!varInitType.equals(varType)) {
            cg.mCtx.error(varDecl,
                          "Variable initializer type mismatch! Variable is of type '%s' and "
                          "initializer is of type '%s'!",
                          varType.name().c_str(),
                          varInitType.name().c_str());
            
            initializerTypeIsOkOut = false;
        }
    }
    
    // Ensure the variable data type is ok
    varTypeIsOkOut = true;
    
    if (!varType.isValid()) {
        cg.mCtx.error(varDecl, "Can't declare a variable of invalid type '%s'!", varType.name().c_str());
        varTypeIsOkOut = false;
    }
    else if (!varType.isSized()) {
        cg.mCtx.error(varDecl, "Can't declare a variable of type '%s' which has no size!", varType.name().c_str());
        varTypeIsOkOut = false;
    }
    else if (!varCompiledType.getLLVMType()) {
        cg.mCtx.error(varDecl, "Internal compiler error! No llvm type generated for variable, can't declare!");
        varTypeIsOkOut = false;
    }
}

/* Codegen the variable declaration as a local variable within a scope */
static void codegenLocalVarDeclWithType(Codegen & cg,
                                        const AST::VarDecl & varDecl,
                                        const CompiledDataType & varCompiledType,
                                        bool varTypeIsInferred)
{
    // Get the current scope, if there is none then issue an error
    const AST::Scope * scope = cg.mCtx.getCurrentScope();
    
    if (!scope) {
        cg.mCtx.error(varDecl, "Internal compiler error! No current scope to code generate variable in!");
    }
    
    // Evaluate the initializer expression:
    varDecl.mInitExpr.accept(cg);
    Value varInitVal = cg.mCtx.popValue();
    
    // Do the type checks for the var decl
    bool initializerTypeIsOk = false;
    bool varTypeIsOk = false;
    
    doVarDeclTypeChecks(cg,
                        varDecl,
                        varCompiledType,
                        varTypeIsInferred,
                        initializerTypeIsOk,
                        varTypeIsOk);
    
    // Can only create the variable if the type and current scope is ok
    WC_GUARD(varTypeIsOk);
    WC_GUARD(scope);
    
    // If the initializer is bad then codegen without it
    if (!initializerTypeIsOk) {
        varInitVal.mLLVMVal = nullptr;
    }
    
    // Makeup the label we will give the var in the IR
    std::string varLabel = kVarLabelPrefix + varDecl.mIdent.name();
    varLabel = StringUtils::appendLineInfo(varLabel.c_str(), varDecl.getStartToken());
    
    // Make the llvm data value for the variable:
    llvm::Value * varAlloca = cg.mCtx.mIRBuilder.CreateAlloca(varCompiledType.getLLVMType(), nullptr, varLabel);
    WC_GUARD(varAlloca);
    
    // Save the data value in the value holder for the scope
    ValHolder & scopeValHolder = cg.mCtx.getScopeValHolder(*scope);
    scopeValHolder.createVal(cg.mCtx,
                             varDecl.mIdent.name(),
                             *varAlloca,
                             varCompiledType,
                             true,
                             varDecl);
    
    // Store the initializer expression to the variable if we generated it ok
    if (varInitVal.mLLVMVal) {
        WC_ASSERTED_OP(cg.mCtx.mIRBuilder.CreateStore(varInitVal.mLLVMVal, varAlloca));
    }
}

/* Codegen the variable declaration as a global variable within the module */
static void codegenGlobalVarDeclWithType(Codegen & cg,
                                         const AST::VarDecl & varDecl,
                                         const CompiledDataType & varCompiledType,
                                         bool varTypeIsInferred)
{
    // Evaluate the initializer expression as a constant
    varDecl.mInitExpr.accept(cg.mConstCodegen);
    Constant varInitVal = cg.mCtx.popConstant();
    
    // Do the type checks for the var decl
    bool initializerTypeIsOk = false;
    bool varTypeIsOk = false;
    
    doVarDeclTypeChecks(cg,
                        varDecl,
                        varCompiledType,
                        varTypeIsInferred,
                        initializerTypeIsOk,
                        varTypeIsOk);
    
    // Can only create the variable if the type is ok
    WC_GUARD(varTypeIsOk);
    
    // If the initializer is bad then codegen without it
    if (!initializerTypeIsOk) {
        varInitVal.mLLVMConst = nullptr;
    }
    
    // TODO: use the linear allocator here
    const char * varName = varDecl.mIdent.name();
    auto * varLLVMVar = new llvm::GlobalVariable(*cg.mCtx.mLLVMModule.get(),
                                                 varCompiledType.getLLVMType(),
                                                 false,                               // Not constant
                                                 llvm::GlobalValue::PrivateLinkage,
                                                 varInitVal.mLLVMConst,
                                                 varName);
    
    WC_ASSERT(varLLVMVar);
    
    // Register the variable. If it's registered more than once then this will generate an error.
    cg.mCtx.mModuleValHolder.createVal(cg.mCtx,
                                       varName,
                                       *varLLVMVar,
                                       varCompiledType,
                                       true,
                                       varDecl);
}

/* Code generate the variable declaration with the given compiled data type */
static void codegenVarDeclWithType(Codegen & cg,
                                   const AST::VarDecl & varDecl,
                                   const CompiledDataType & varCompiledType,
                                   bool varTypeIsInferred)
{
    // Codegen as either a local or global variable definition
    if (cg.mCtx.mCurFunction) {
        codegenLocalVarDeclWithType(cg,
                                    varDecl,
                                    varCompiledType,
                                    varTypeIsInferred);
    }
    else {
        codegenGlobalVarDeclWithType(cg,
                                     varDecl,
                                     varCompiledType, 
                                     varTypeIsInferred);
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
    astNode.mInitExpr.dataType().accept(mCodegenDataType);
    CompiledDataType varCompiledType = mCtx.popCompiledDataType();
    codegenVarDeclWithType(*this, astNode, varCompiledType, true);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE