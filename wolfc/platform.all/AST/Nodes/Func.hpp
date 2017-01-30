#pragma once

#include "ASTNode.hpp"
#include "CStrComparator.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
    #include <map>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

class DataType;

WC_AST_BEGIN_NAMESPACE

class FuncArg;
class Identifier;
class Scope;
class Type;

/*
Func:
	func Identifier ( [0..N: FuncArg ,][FuncArg] ) [-> Type] Scope end
*/
class Func final : public ASTNode {
public:
    static bool peek(const Token * tokenPtr);
    static Func * parse(ParseCtx & parseCtx);
    
    Func(const Token & startToken,
         Identifier & identifier,
         std::vector<FuncArg*> && funcArgs,
         Type * returnType,
         Scope & scope,
         const Token & endToken);
    
    virtual void accept(ASTNodeVisitor & visitor) const override;
    virtual const Token & getStartToken() const override;
    virtual const Token & getEndToken() const override;
    
    const char * name() const;
    
    inline const std::vector<FuncArg*> & getArgs() const {
        return mFuncArgs;
    }
    
    const DataType & returnDataType() const;
    
    const Token &       mStartToken;
    Identifier &        mIdentifier;
    Scope &             mScope;
    const Token &       mEndToken;
    
private:
    /* The list of function arguments parsed */
    std::vector<FuncArg*> mFuncArgs;
    
    /**
     * The explicitly specified return type for this function. 
     * If this is null then void type is assumed.
     */
    Type * mReturnType;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
