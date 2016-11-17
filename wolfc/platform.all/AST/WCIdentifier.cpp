#include "WCIdentifier.hpp"

#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCFunc.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
#include "WCPrimitiveDataTypes.hpp"
#include "WCScope.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool Identifier::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIdentifier;
}

Identifier * Identifier::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    if (tokenPtr->type != TokenType::kIdentifier) {
        parseError(*tokenPtr, "Expected identifier!");
        return nullptr;
    }
    
    Identifier * intLit = WC_NEW_AST_NODE(alloc, Identifier, *tokenPtr);
    ++tokenPtr;
    return intLit;
}

Identifier::Identifier(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

const Token & Identifier::getStartToken() const {
    return mToken;
}

const Token & Identifier::getEndToken() const {
    return mToken;
}

bool Identifier::isLValue() {
    return true;
}

bool Identifier::isConstExpr() {
    // TODO: Implement this
    return false;
}

DataType & Identifier::dataType() {
    DataValue * dataValue = lookupDataValue();
    
    if (dataValue) {
        WC_ASSERT(dataValue->type);
        return *dataValue->type;
    }
    
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kUnknown);
}

llvm::Value * Identifier::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    
    #warning Does this work for function arguments?
    // Grab the variable value
    const DataValue * dataValue = lookupDataValue();
    
    if (!dataValue) {
        compileError("Can't codegen address of variable '%s'! No such variable in current scope!", name());
        return nullptr;
    }
    
    // Return it!
    WC_ASSERT(dataValue->value);
    return dataValue->value;
}

llvm::Value * Identifier::codegenExprEval(CodegenCtx & cgCtx) {
    // Grab the variable value
    const DataValue * dataValue = lookupDataValue();
    
    if (!dataValue) {
        compileError("No variable named '%s' in the current scope! Unable to take it's value!", name());
        return nullptr;
    }
    
    // TODO: this won't work for anything other than primitives
    
    // Create an instruction to load it, if required, and return that instead of the value:
    WC_ASSERT(dataValue->value);
    
    if (dataValue->requiresLoad) {
        return cgCtx.irBuilder.CreateLoad(dataValue->value, std::string("load_ident_val:") + name());
    }
    
    // No load required, return directly:
    return dataValue->value;
}

llvm::Constant * Identifier::codegenExprConstEval(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    
    // Grab the variable value
    const DataValue * dataValue = lookupDataValue();
    
    if (!dataValue) {
        compileError("No constant named '%s' in the current scope! Unable to take it's value!", name());
        return nullptr;
    }
    
    // See if it evaluates to a global variable:
    if (llvm::GlobalVariable * globalVar = llvm::dyn_cast<llvm::GlobalVariable>(dataValue->value)) {
        // If must have a constant initializer for us to use it's value:
        if (llvm::Constant * globalVarInitializer = globalVar->getInitializer()) {
            return globalVarInitializer;
        }
    }
    
    // TODO: relax restriction and allow referencing constant variables which have been defined
    compileError("The variable named '%s' cannot be referenced in a constant expression because "
                 "it cannot be evaluated at compile time!",
                 name());
    
    return nullptr;
}

const char * Identifier::name() const {
    return mToken.data.strVal.ptr;
}

DataValue * Identifier::lookupDataValue() {
    // See if there is a parent scope, if so then try to lookup the value within that and
    // all parent scopes of that parent scope...
    const char * identifierName = name();
    
    {
        Scope * parentScope = getParentScope();
        
        while (parentScope) {
            DataValue * dataValue = parentScope->getVar(identifierName);
            
            if (dataValue) {
                return dataValue;
            }
            
            parentScope = parentScope->getParentScope();
        }
    }
    
    // Failing that check if there is a parent function and try to lookup the value within that:
    {
        Func * parentFunc = getParentFunc();
        
        if (parentFunc) {
            DataValue * dataValue = parentFunc->getArg(identifierName);
            
            if (dataValue) {
                return dataValue;
            }
        }
    }
    
    // Failing that get the parent module and try to lookup the value within that:
    Module * parentModule = getParentModule();
    WC_ASSERT(parentModule);
    return parentModule->getVar(identifierName);
}

WC_END_NAMESPACE
