#include "WCVarDecl.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssignExpr.hpp"
#include "WCCodegenCtx.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
#include "WCScope.hpp"
#include "WCType.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// VarDecl
//-----------------------------------------------------------------------------

bool VarDecl::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kVar;
}

VarDecl * VarDecl::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    // Parse 'var' keyword
    if (tokenPtr->type != TokenType::kVar) {
        // TODO: add 'let' to docs here?
        parseError(*tokenPtr, "Expected keyword 'var' at start of variable declaration!");
        return nullptr;
    }
    
    const Token * varToken = tokenPtr;
    ++tokenPtr;     // Skip 'var'
    
    // Parse the identifier ahead
    Identifier * ident = Identifier::parse(tokenPtr, alloc);
    WC_GUARD(ident, nullptr);
    
    // See if the type for the variable is specified:
    Type * type = nullptr;
    
    if (tokenPtr->type == TokenType::kColon) {
        // Type specified, skip the ':'
        ++tokenPtr;
        
        // Parse the type:
        type = Type::parse(tokenPtr, alloc);
        WC_GUARD(type, nullptr);
    }
    
    // Parse the '='
    if (tokenPtr->type != TokenType::kAssign) {
        parseError(*tokenPtr, "Expected '=' following variable name for variable declaration!");
        return nullptr;
    }
    
    ++tokenPtr;
    
    // Parse the init expression and return result of parsing
    AssignExpr * initExpr = AssignExpr::parse(tokenPtr, alloc);
    WC_GUARD(initExpr, nullptr);
    
    // Now return the AST node:
    if (type) {
        return WC_NEW_AST_NODE(alloc, VarDeclExplicitType, *varToken, *ident, *type, *initExpr);
    }
    else {
        return WC_NEW_AST_NODE(alloc, VarDeclInferType, *varToken, *ident, *initExpr);
    }
}

VarDecl::VarDecl(const Token & token,
                 Identifier & ident,
                 AssignExpr & initExpr)
:
    mStartToken(token),
    mIdent(ident),
    mInitExpr(initExpr)
{
    mIdent.mParent = this;
    mInitExpr.mParent = this;
}

const Token & VarDecl::getStartToken() const {
    return mStartToken;
}

const Token & VarDecl::getEndToken() const {
    return mInitExpr.getEndToken();
}

bool VarDecl::codegen(CodegenCtx & cgCtx) {
    // See if this is a local or global var. If it is a local var then there will be a parent scope:
    Scope * parentScope = getParentScope();
    
    if (parentScope) {
        return codegenAsLocalVar(cgCtx, *parentScope);     // Local variable!
    }
    
    return codegenAsGlobalVar(cgCtx);
}

bool VarDecl::allCodepathsHaveUncondRet() const {
    return false;
}

bool VarDecl::codegenAsLocalVar(CodegenCtx & cgCtx, Scope & parentScope) {
    // Create the variable. If this fails then the variable already exists:
    DataType & varDataType = dataType();
    DataValue * leftValue = parentScope.createVar(mIdent.mToken.data.strVal.ptr,
                                                  varDataType,
                                                  cgCtx,
                                                  *this);
    WC_GUARD(leftValue, nullptr);
    
    // Now evaluate the right:
    llvm::Value * rightValue = mInitExpr.codegenExprEval(cgCtx);
    WC_GUARD(rightValue, false);
    
    // Data type for the var must equal the data type of the expression:
    DataType & exprDataType = mInitExpr.dataType();
    
    if (!varDataType.equals(exprDataType)) {
        // TODO: Handle auto type promotion here
        compileError("Initializing expression for variable declaration must be of type '%s', not '%s'!",
                     varDataType.name().c_str(),
                     exprDataType.name().c_str());
        
        return false;
    }

    // Generate store instruction:
    return cgCtx.irBuilder.CreateStore(rightValue, leftValue->value) != nullptr;
}

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

//-----------------------------------------------------------------------------
// VarDeclInferType
//-----------------------------------------------------------------------------

VarDeclInferType::VarDeclInferType(const Token & startToken,
                                   Identifier & ident,
                                   AssignExpr & initExpr)
:
    VarDecl(startToken, ident, initExpr)
{
    WC_EMPTY_FUNC_BODY();
}

DataType & VarDeclInferType::dataType() const {
    return mInitExpr.dataType();
}

//-----------------------------------------------------------------------------
// VarDeclExplicitType
//-----------------------------------------------------------------------------

VarDeclExplicitType::VarDeclExplicitType(const Token & startToken,
                                         Identifier & ident,
                                         Type & type,
                                         AssignExpr & initExpr)
:
    VarDecl(startToken, ident, initExpr),
    mType(type)
{
    mType.mParent = this;
}

DataType & VarDeclExplicitType::dataType() const {
    return mType.dataType();
}

WC_END_NAMESPACE
