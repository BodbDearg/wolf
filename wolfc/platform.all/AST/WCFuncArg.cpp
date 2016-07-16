#include "WCFuncArg.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCPrimitiveType.hpp"
#include "WCToken.hpp"

WC_BEGIN_NAMESPACE

bool FuncArg::peek(const Token * currentToken) {
    return PrimitiveType::peek(currentToken);
}

FuncArg * FuncArg::parse(const Token *& currentToken, LinearAlloc & alloc) {
    // Check stuff ahead
    if (!peek(currentToken)) {
        parseError(*currentToken, "Expected function argument!");
        return nullptr;
    }
    
    // TODO: support non primitive types here eventually (struct etc.)
    // TODO: support arrays eventually
    // TODO: support pointers eventually
    
    // Parse the data type
    PrimitiveType * dataType = PrimitiveType::parse(currentToken, alloc);
    WC_GUARD(dataType, nullptr);
    
    // Parse the identifier following:
    Identifier * identifier = Identifier::parse(currentToken, alloc);
    WC_GUARD(identifier, nullptr);
    
    // Success, return the node:
    return WC_NEW_AST_NODE(alloc, FuncArg, *dataType, *identifier);
}

FuncArg::FuncArg(PrimitiveType & dataType, Identifier & identifier) :
    mDataType(dataType),
    mIdentifier(identifier)
{
    mDataType.mParent = this;
    mIdentifier.mParent = this;
}

const Token & FuncArg::getStartToken() const {
    return mDataType.getStartToken();
}

const Token & FuncArg::getEndToken() const {
    return mIdentifier.getEndToken();
}

const DataType & FuncArg::dataType() const {
    return mDataType.dataType();
}

const char * FuncArg::name() const {
    return mIdentifier.name();
}

WC_END_NAMESPACE
