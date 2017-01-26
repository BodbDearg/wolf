#pragma once

#include "WCAssert.hpp"
#include "WCASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <map>
    #include <memory>
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class ParseCtx;

WC_AST_BEGIN_NAMESPACE

class DeclDef;
class Func;
class VarDecl;

/* 
Module:
    [0..N: DeclDef] EOF
*/
class Module final : public ASTNode {
public:
    /* Parse the code for the module from the given parse context */
    static Module * parse(ParseCtx & parseCtx);
    
    Module(std::vector<DeclDef*> && mDeclDefs, const Token & eofToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    /* All declarations and definitions in the module/ */
    const std::vector<DeclDef*> mDeclDefs;
    
private:
    /* The EOF token */
    const Token & mEOFToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
