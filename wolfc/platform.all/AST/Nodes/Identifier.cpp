#include "Identifier.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/UnevalDataType.hpp"
#include "LinearAlloc.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool Identifier::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kIdentifier;
}

Identifier * Identifier::parse(ParseCtx & parseCtx) {
    if (parseCtx.tok()->type != TokenType::kIdentifier) {
        parseCtx.error("Expected identifier!");
        return nullptr;
    }
    
    Identifier * intLit = WC_NEW_AST_NODE(parseCtx, Identifier, *parseCtx.tok());
    parseCtx.nextTok();
    return intLit;
}

Identifier::Identifier(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void Identifier::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & Identifier::getStartToken() const {
    return mToken;
}

const Token & Identifier::getEndToken() const {
    return mToken;
}

bool Identifier::isLValue() const {
    return true;
}

bool Identifier::isConstExpr() const {
    // FIXME: Implement this
    return false;
}

const DataType & Identifier::dataType() const {
    // We won't know this until further analysis at compile time
    return PrimitiveDataTypes::getUnevalDataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * Identifier::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    
    // FIXME: Does this work for function arguments?
    // Grab the variable value
    const DataValue * dataValue = lookupDataValue();
    
    if (!dataValue) {
        compileError("Can't codegen address of variable '%s'! No such variable in current scope!", name());
        return nullptr;
    }
    
    // Get the LLVM value and see if it is a pointer.
    // If we already have a pointer then we don't need to do anything else to get the address.
    llvm::Value * llvmValue = dataValue->value;
    WC_ASSERT(llvmValue);

    if (llvmValue->getType()->isPointerTy()) {
        return llvmValue;
    }
    
    // Issue an error
    compileError("Can't codegen address of variable '%s'! "
                 "Dealing with an LLVM type that can't be converted to an address!", 
                 name());

    return nullptr;
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
#endif

const char * Identifier::name() const {
    return mToken.data.strVal.ptr;
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
