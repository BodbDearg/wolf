#include "WCFuncArg.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCParseCtx.hpp"
#include "WCType.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool FuncArg::peek(const Token * currentToken) {
    return Identifier::peek(currentToken);
}

FuncArg * FuncArg::parse(ParseCtx & parseCtx) {
    // TODO: support non primitive types here eventually (struct etc.)
    // TODO: support arrays eventually
    // TODO: support pointers eventually
    
    // Parse the identifier:
    Identifier * ident = Identifier::parse(parseCtx);
    WC_GUARD(ident, nullptr);
    
    // Expect ':' following the identifier
    if (parseCtx.tok()->type != TokenType::kColon) {
        parseCtx.error("expect ':' following argument name for function argument!");
        return nullptr;
    }
    
    // Skip ':'
    parseCtx.nextTok();
    
    // Parse the data type
    Type * type = Type::parse(parseCtx);
    WC_GUARD(type, nullptr);
    
    // Success, return the node:
    return WC_NEW_AST_NODE(parseCtx, FuncArg, *type, *ident);
}

FuncArg::FuncArg(Type & type, Identifier & ident) :
    mType(type),
    mIdent(ident)
{
    mType.mParent = this;
    mIdent.mParent = this;
}

void FuncArg::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
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

#warning FIXME - Codegen
#if 0
bool FuncArg::codegen(CodegenCtx & cgCtx, ASTNode & callingNode) {
    return mType.codegenLLVMType(cgCtx, callingNode);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE