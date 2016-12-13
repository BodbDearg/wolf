#include "WCScope.hpp"

#include "DataType/WCDataType.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCStmnt.hpp"
#include "WCStringUtils.hpp"
#include "WCVarDecl.hpp"

WC_BEGIN_NAMESPACE

Scope * Scope::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Save start token:
    const Token * startToken = tokenPtr;
    
    // Parse all statements we can:
    std::vector<Stmnt*> stmnts;
    
    while (Stmnt::peek(tokenPtr)) {
        Stmnt * stmnt = Stmnt::parse(tokenPtr, alloc);
        WC_GUARD(stmnt, nullptr);
        stmnts.push_back(stmnt);
    }
    
    // Return the parsed scope
    return WC_NEW_AST_NODE(alloc, Scope, *startToken, std::move(stmnts));
}

Scope::Scope(const Token & startToken, std::vector<Stmnt*> && stmnts)
:
    mStartToken(startToken),
    mStmnts(stmnts)
{
    for (Stmnt * stmnt : mStmnts) {
        stmnt->mParent = this;
    }
}

const Token & Scope::getStartToken() const {
    if (!mStmnts.empty()) {
        return mStmnts.front()->getStartToken();
    }
    
    return mStartToken;
}

const Token & Scope::getEndToken() const {
    if (!mStmnts.empty()) {
        return mStmnts.back()->getEndToken();
    }
    
    return mStartToken;
}

bool Scope::codegen(CodegenCtx & cgCtx) {
    for (Stmnt * stmnt : mStmnts) {
        WC_GUARD(stmnt->codegen(cgCtx), false);
    }
    
    return true;
}

bool Scope::allCodepathsHaveUncondRet() const {
    for (Stmnt * stmnt : mStmnts) {
        if (stmnt->allCodepathsHaveUncondRet()) {
            return true;
        }
    }
    
    return false;
}

DataValue * Scope::createVar(const char * varName,
                             DataType & dataType,
                             CodegenCtx & cgCtx,
                             VarDecl & callingNode)
{
    // If the variable already exists in this scope then creation fails:
    {
        auto iter = mVarValues.find(varName);
        
        if (iter != mVarValues.end()) {
            compileError("Attempting to redeclare variable '%s' in this scope!", varName);
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
    dataValue.declaringNode = &callingNode;
    dataValue.requiresLoad = true;
    dataValue.type = &dataType;
    dataValue.value = dataType.codegenAlloca(cgCtx, *this, std::string("alloc_ident_val:") + varName);
    WC_GUARD(dataValue.value, nullptr);

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
