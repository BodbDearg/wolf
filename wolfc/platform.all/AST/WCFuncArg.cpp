#include "WCFuncArg.hpp"

#include "Lexer/WCToken.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCType.hpp"

WC_BEGIN_NAMESPACE

bool FuncArg::peek(const Token * currentToken) {
    return Identifier::peek(currentToken);
}

FuncArg * FuncArg::parse(const Token *& currentToken, LinearAlloc & alloc) {
    // TODO: support non primitive types here eventually (struct etc.)
    // TODO: support arrays eventually
    // TODO: support pointers eventually
    
    // Parse the identifier:
    Identifier * ident = Identifier::parse(currentToken, alloc);
    WC_GUARD(ident, nullptr);
    
    // Expect ':' following the identifier
    if (currentToken->type != TokenType::kColon) {
        parseError(*currentToken, "expect ':' following argument name for function argument!");
        return nullptr;
    }
    
    // Skip ':'
    ++currentToken;
    
    // Parse the data type
    Type * type = Type::parse(currentToken, alloc);
    WC_GUARD(type, nullptr);
    
    // Success, return the node:
    return WC_NEW_AST_NODE(alloc, FuncArg, *type, *ident);
}

FuncArg::FuncArg(Type & type, Identifier & ident) :
    mType(type),
    mIdent(ident)
{
    mType.mParent = this;
    mIdent.mParent = this;
}

const Token & FuncArg::getStartToken() const {
    return mType.getStartToken();
}

const Token & FuncArg::getEndToken() const {
    return mIdent.getEndToken();
}

DataType & FuncArg::dataType() {
    return mType.dataType();
}

const char * FuncArg::name() const {
    return mIdent.name();
}

bool FuncArg::codegen(CodegenCtx & cgCtx, ASTNode & callingNode) {
    return mType.codegenLLVMType(cgCtx, callingNode);
}

WC_END_NAMESPACE
