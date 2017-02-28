//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "ASTNode.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "Func.hpp"
#include "Module.hpp"
#include "Scope.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdarg>
    #include <cstdio>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

ASTNode::ASTNode() : mParent(nullptr) {
    WC_EMPTY_FUNC_BODY();
}

ASTNode::~ASTNode() {
    WC_EMPTY_FUNC_BODY();
}

const Token & ASTNode::getPastEndToken() const {
    const Token & endToken = getEndToken();
    
    if (endToken.type == TokenType::kEOF) {
        return endToken;
    }
    
    return (&endToken)[1];
}

Scope * ASTNode::getParentScope() {
    return firstParentOfType<Scope>();
}

const Scope * ASTNode::getParentScope() const {
    return firstParentOfType<Scope>();
}

Func * ASTNode::getParentFunc() {
    return firstParentOfType<Func>();
}

const Func * ASTNode::getParentFunc() const {
    return firstParentOfType<Func>();
}

Module * ASTNode::getParentModule() {
    return firstParentOfType<Module>();
}

const Module * ASTNode::getParentModule() const {
    return firstParentOfType<Module>();
}

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
