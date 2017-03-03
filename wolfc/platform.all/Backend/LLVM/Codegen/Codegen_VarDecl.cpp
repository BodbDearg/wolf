//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "Codegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "../ImplicitCasts.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/Identifier.hpp"
#include "AST/Nodes/Scope.hpp"
#include "AST/Nodes/Type.hpp"
#include "AST/Nodes/VarDecl.hpp"
#include "Assert.hpp"
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
                                const CompiledDataType * varExplicitType,
                                const CompiledDataType & varInitializerType,
                                bool & varInitializerTypeIsOkOut,
                                bool & varTypeIsOkOut)
{
    // Get the var data type
    const CompiledDataType & varCompiledType = varExplicitType ? *varExplicitType : varInitializerType;
    const DataType & varDataType = varCompiledType.getDataType();
    
    // Ensure the initializer is the same type as the var decl if the var type is not inferred.
    // If we are not using inference there might be a mismatch...
    if (varExplicitType) {
        // Var type is not inferred, initializer must be the same type as the explicitly specified var type
        varInitializerTypeIsOkOut = true;
        
        // Make sure the initializer and variable types match
        const DataType & varInitializerDataType = varInitializerType.getDataType();
        
        if (!varInitializerDataType.equals(varDataType)) {
            cg.mCtx.error(varDecl,
                          "Variable initializer type mismatch! Variable is of type '%s' and "
                          "initializer is of type '%s'!",
                          varDataType.name().c_str(),
                          varInitializerDataType.name().c_str());
            
            varInitializerTypeIsOkOut = false;
        }
    }
    else {
        // Var type is inferred, therefore the initializer type is always ok
        varInitializerTypeIsOkOut = true;
    }
    
    // Ensure the variable data type is ok
    varTypeIsOkOut = true;
    
    if (!varDataType.isValid()) {
        // Note: no error for an invalid type because we've already done that elsewhere - the error is redundant
        varTypeIsOkOut = false;
    }
    else if (!varDataType.isSized()) {
        cg.mCtx.error(varDecl, "Can't declare a variable of type '%s' which has no size!", varDataType.name().c_str());
        varTypeIsOkOut = false;
    }
    else if (!varCompiledType.getLLVMType()) {
        cg.mCtx.error(varDecl, "Internal compiler error! No llvm type generated for variable, can't declare!");
        varTypeIsOkOut = false;
    }
}

/* Codegen the variable declaration as a local variable within a scope */
static void codegenLocalVarDecl(Codegen & cg,
                                const AST::VarDecl & varDecl,
                                const CompiledDataType * varExplicitType)
{
    // Get the current scope, if there is none then issue an error
    const AST::Scope * scope = cg.mCtx.getCurrentScope();
    
    if (!scope) {
        cg.mCtx.error(varDecl, "Internal compiler error! No current scope to code generate variable in!");
    }
    
    // Evaluate the initializer expression:
    varDecl.mInitExpr.accept(cg);
    Value varInitVal = cg.mCtx.popValue();
    
    // The type of the variable is either the explicitly specified type or based on the
    // initializer expression type (if the var type is inferred)
    const CompiledDataType & varCompiledType = varExplicitType ?
        *varExplicitType :
        varInitVal.mCompiledType;
    
    // If possible, do any implicit casts that may be required for the initial value:
    varInitVal = ImplicitCasts::castSingleValueIfRequired(cg,
                                                          varInitVal,
                                                          varCompiledType);
    
    // Do the type checks for the var decl
    bool varInitializerTypeIsOk = false;
    bool varTypeIsOk = false;
    
    doVarDeclTypeChecks(cg,
                        varDecl,
                        varExplicitType,
                        varInitVal.mCompiledType,
                        varInitializerTypeIsOk,
                        varTypeIsOk);
    
    // Can only create the variable if the type and current scope is ok
    WC_GUARD(varTypeIsOk);
    WC_GUARD(scope);
    
    // If the initializer is bad then codegen without it
    if (!varInitializerTypeIsOk) {
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
                             varAlloca,
                             varCompiledType,
                             true,
                             varDecl,
                             false);
    
    // Store the initializer expression to the variable if we generated it ok
    if (varInitVal.mLLVMVal) {
        WC_ASSERTED_OP(cg.mCtx.mIRBuilder.CreateStore(varInitVal.mLLVMVal, varAlloca));
    }
}

/* Codegen the variable declaration as a global variable within the module */
static void codegenGlobalVarDecl(Codegen & cg,
                                 const AST::VarDecl & varDecl,
                                 const CompiledDataType * varExplicitType)
{
    // Evaluate the initializer expression as a constant
    varDecl.mInitExpr.accept(cg.mConstCodegen);
    Constant varInitConst = cg.mCtx.popConstant();
    
    // The type of the variable is either the explicitly specified type or based on the
    // initializer expression type (if the var type is inferred)
    const CompiledDataType & varCompiledType = varExplicitType ?
        *varExplicitType :
        varInitConst.mCompiledType;
    
    // If possible, do any implicit casts that may be required for the initial value:
    varInitConst = ImplicitCasts::castSingleConstantIfRequired(cg.mConstCodegen,
                                                               varInitConst,
                                                               varCompiledType);
    
    // Do the type checks for the var decl
    bool varInitializerTypeIsOk = false;
    bool varTypeIsOk = false;
    
    doVarDeclTypeChecks(cg,
                        varDecl,
                        varExplicitType,
                        varInitConst.mCompiledType,
                        varInitializerTypeIsOk,
                        varTypeIsOk);
    
    // Can only create the variable if the type is ok
    WC_GUARD(varTypeIsOk);
    
    // If the initializer is bad then codegen without it
    if (!varInitializerTypeIsOk) {
        varInitConst.mLLVMConst = nullptr;
    }
    
    // Register the variable. If it's registered more than once then this will generate an error.
    // We'll fill in the llvm value later once the unique name has been determined...
    Value & value = cg.mCtx.mModuleValHolder.createVal(
        cg.mCtx,
        varDecl.mIdent.name(),
        nullptr,
        varCompiledType,
        true,
        varDecl,
        false
    );
    
    // Create the llvm value.
    // TODO: use the linear allocator here
    value.mLLVMVal = new llvm::GlobalVariable(
        *cg.mCtx.mLLVMModule.get(),
        varCompiledType.getLLVMType(),
        false,                               // Not constant
        llvm::GlobalValue::PrivateLinkage,
        varInitConst.mLLVMConst,
        value.mName
    );
    
    WC_ASSERT(value.mLLVMVal);
    
    // Register it as a constant that can be used also.
    // Note: skip duplicate name checks in this case, because we already done for the variable declaration.
    cg.mCtx.mModuleValHolder.createConst(
        cg.mCtx,
        varDecl.mIdent.name(),
        varInitConst.mLLVMConst,
        varCompiledType,
        varDecl,
        true
    );
}

/**
 * Code generate the variable declaration with the given explicitly specified data type for the variable,
 * which may be null. If the compiled data type is not given, then the data type for the variable is 
 * infered from the variable initializer expression.
 */
static void codegenVarDecl(Codegen & cg,
                           const AST::VarDecl & varDecl,
                           const CompiledDataType * varExplicitType)
{
    // Codegen as either a local or global variable definition.
    // If we are currently code generating a function then codegen as a local var.
    if (cg.mCtx.mCurFunc) {
        codegenLocalVarDecl(cg, varDecl, varExplicitType);
    }
    else {
        codegenGlobalVarDecl(cg, varDecl, varExplicitType);
    }
}

void Codegen::visit(const AST::VarDeclExplicitType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the var with the given explictly given data type:
    astNode.mType.accept(*this);
    CompiledDataType varExplicitType = mCtx.popCompiledDataType();
    codegenVarDecl(*this, astNode, &varExplicitType);
}

void Codegen::visit(const AST::VarDeclInferType & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the var but infer the data type:
    codegenVarDecl(*this, astNode, nullptr);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
