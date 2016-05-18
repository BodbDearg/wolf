#include "WCPrimaryExpr.hpp"
#include "WCIdentifier.hpp"
#include "WCIntLit.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool PrimaryExpr::peek(const Token * currentToken) {
    if (currentToken->type == TokenType::kIntLit ||
        currentToken->type == TokenType::kIdentifier)
    {
        return true;
    }
    
    return false;
}

PrimaryExpr * PrimaryExpr::parse(const Token *& currentToken) {
    switch (currentToken->type) {
        /* IntLit */
        case TokenType::kIntLit: {
            IntLit * uintLit = IntLit::parse(currentToken);
            WC_GUARD(uintLit, nullptr);
            return new PrimaryExprIntLit(*uintLit);
        }   break;
            
        /* Identifier */
        case TokenType::kIdentifier: {
            Identifier * identifier = Identifier::parse(currentToken);
            WC_GUARD(identifier, nullptr);
            return new PrimaryExprIdentifier(*identifier);
        }   break;
            
        default:
            break;
    }
    
    error(*currentToken, "Expected primary expression!");
    return nullptr;
}

PrimaryExprIntLit::PrimaryExprIntLit(IntLit & lit) : mLit(lit) {
    mLit.mParent = this;
}

llvm::Value * PrimaryExprIntLit::generateCode(const CodegenCtx & cgCtx) {
    return mLit.generateCode(cgCtx);
}

bool PrimaryExprIntLit::isLValue() const {
    return false;
}

llvm::Value * PrimaryExprIntLit::codegenAddrOf(const CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    return nullptr;
}

PrimaryExprIdentifier::PrimaryExprIdentifier(Identifier & identifier) : mIdentifier(identifier) {
    mIdentifier.mParent = this;
}

llvm::Value * PrimaryExprIdentifier::generateCode(const CodegenCtx & cgCtx) {
    return mIdentifier.generateCode(cgCtx);
}

llvm::Value * PrimaryExprIdentifier::codegenAddrOf(const CodegenCtx & cgCtx) {
    return mIdentifier.codegenAddrOf(cgCtx);
}

bool PrimaryExprIdentifier::isLValue() const {
    return true;
}

WC_END_NAMESPACE
