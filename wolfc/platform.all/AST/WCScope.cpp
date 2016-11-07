#include "WCScope.hpp"
#include "WCCodegenCtx.hpp"
#include "WCDataType.hpp"
#include "WCLinearAlloc.hpp"
#include "WCStmnts.hpp"
#include "WCStringUtils.hpp"

WC_BEGIN_NAMESPACE

bool Scope::peek(const Token * tokenPtr) {
    return Stmnts::peek(tokenPtr);
}

Scope * Scope::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    Stmnts * stmnts = Stmnts::parse(tokenPtr, alloc);
    WC_GUARD(stmnts, nullptr);
    return WC_NEW_AST_NODE(alloc, Scope, *stmnts);
}

Scope::Scope(Stmnts & stmnts) : mStmnts(stmnts) {
    mStmnts.mParent = this;
}

const Token & Scope::getStartToken() const {
    return mStmnts.getStartToken();
}

const Token & Scope::getEndToken() const {
    return mStmnts.getEndToken();
}

bool Scope::codegen(CodegenCtx & cgCtx) {
    return mStmnts.codegen(cgCtx);
}

DataValue * Scope::createVar(const char * varName,
                             DataType & dataType,
                             CodegenCtx & cgCtx,
                             ASTNode & callingNode)
{
    // If the variable already exists in this scope then creation fails:
    {
        auto iter = mVarValues.find(varName);
        
        if (iter != mVarValues.end()) {
            return nullptr;
        }
    }
    
    // Compile the variable llvm type:
    WC_GUARD(dataType.codegenLLVMTypeIfRequired(cgCtx, callingNode), nullptr);
    
    // The variable must have an llvm type:
    if (!dataType.mLLVMType) {
        compileError("Variable '%s' of type '%s' is not an llvm primitive type! Cannot create a variable to hold it!",
                     varName,
                     dataType.name().c_str());
        
        return nullptr;
    }
    
    // Make the data value:
    DataValue & dataValue = mVarValues[varName];
    dataValue.requiresLoad = true;
    dataValue.type = &dataType;
    dataValue.value = dataType.codegenAlloca(cgCtx, *this, std::string("alloc_ident_val:") + varName);
    
    if (!dataValue.value) {
        compileError("Failed to codegen a stack allocation for the variable '%s' of type '%s'!",
                     varName,
                     dataType.name().c_str());
        
        return nullptr;
    }
    
    return &dataValue;
}

DataValue * Scope::getVar(const char * varName) {
    // First search in this scope:
    auto iter = mVarValues.find(varName);
    
    if (iter != mVarValues.end()) {
        return &iter->second;
    }
    
    // If that fails try a parent scope, if it exists:
    Scope * parentScope = getParentScope();
    
    if (parentScope) {
        return parentScope->getVar(varName);
    }
    
    return nullptr;     // Variable not found!
}

WC_END_NAMESPACE
