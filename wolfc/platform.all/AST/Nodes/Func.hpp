//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "ASTNode.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class FuncArg;
class Identifier;
class Scope;
class Type;

/*
Func:
	func Identifier ( [0..N: FuncArg ,][FuncArg] ) [-> Type] [do] Scope end
*/
class Func final : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static Func * parse(ParseCtx & parseCtx);
    
    Func(const Token & startToken,
         Identifier & identifier,
         std::vector<FuncArg*> && funcArgs,
         Type * explicitReturnType,
         Scope & scope,
         const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const char * name() const;
    
    inline const Identifier & getIdentifier() const {
        return mIdentifier;
    }
    
    inline const std::vector<FuncArg*> & getArgs() const {
        return mFuncArgs;
    }
    
    inline const Type * getExplicitReturnType() const {
        return mExplicitReturnType;
    }
    
    inline const Scope & getScope() const {
        return mScope;
    }
    
private:
    /* Start token for the function */
    const Token & mStartToken;
    
    /* The function name identifier */
    Identifier & mIdentifier;
    
    /* The list of function arguments parsed */
    std::vector<FuncArg*> mFuncArgs;
    
    /**
     * The explicitly specified return type for this function.
     * If this is null then void type is assumed.
     */
    Type * mExplicitReturnType;
    
    /* The inner body scope of the function */
    Scope & mScope;
    
    /* The end token for the function */
    const Token & mEndToken;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
