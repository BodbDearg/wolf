#include "WCASTNode.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "WCFunc.hpp"
#include "WCModule.hpp"
#include "WCParseCtx.hpp"
#include "WCScope.hpp"

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
