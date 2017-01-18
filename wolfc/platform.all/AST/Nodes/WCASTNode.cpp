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

#warning FIXME - Codegen
#if 0
void ASTNode::compileError(const char * msg, ...) const {
    // Get the start and end tokens:
    const Token & startToken = getStartToken();
    const Token & endToken = getEndToken();
    
    // Generic error info
    std::fprintf(stderr,
                 "Error! Compile failed for the code beginning at line %zu, column %zu and "
                 "ending at line %zu and column %zu! Message:\n",
                 startToken.startLine + 1,
                 startToken.startCol + 1,
                 endToken.endLine + 1,
                 endToken.endCol + 1);
    
    // Specific error info
    std::va_list args;
    va_start(args, msg);
    std::vfprintf(stderr, msg, args);
    va_end(args);
    std::fprintf(stderr, "\n");
}

std::string ASTNode::makeLLVMLabelForTok(const char * labelText, const Token & token) {
    WC_ASSERT(labelText);
    std::string label = labelText;
    label += "@l";
    label += std::to_string(token.startLine + 1);
    label += ", ch";
    label += std::to_string(token.startCol + 1);
    return label;
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
