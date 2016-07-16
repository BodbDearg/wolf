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

const DataValue * Scope::createVariable(const char * variableName,
                                        const DataType & dataType,
                                        CodegenCtx & cgCtx)
{
    // If the variable already exists in this scope then creation fails:
    {
        auto iter = mVariableValues.find(variableName);
        
        if (iter != mVariableValues.end()) {
            return nullptr;
        }
    }
    
    // TODO: support more complex variable types other than llvm primitives
    // The variable must have an llvm type:
    llvm::Type * llvmType = dataType.llvmType(cgCtx);
    
    if (!llvmType) {
        compileError("Variable '%s' of type '%s' is not an llvm primitive type! Cannot create a variable to hold it!",
                     variableName,
                     dataType.name());
        
        return nullptr;
    }
    
    // Otherwise make it:
    DataValue & dataValue = mVariableValues[variableName];
    
    dataValue.type = &dataType;
    dataValue.value = cgCtx.irBuilder.CreateAlloca(llvmType,
                                                   nullptr,
                                                   std::string("alloc_ident_val:") + variableName);
    
    return &dataValue;
}

const DataValue * Scope::getVariable(const char * variableName) const {
    // First search in this scope:
    auto iter = mVariableValues.find(variableName);
    
    if (iter != mVariableValues.end()) {
        return &iter->second;
    }
    
    // If that fails try a parent scope:
    const Scope * parentScope = getParentScope();
    
    if (parentScope) {
        return parentScope->getVariable(variableName);
    }
    
    return nullptr;
}

WC_END_NAMESPACE
