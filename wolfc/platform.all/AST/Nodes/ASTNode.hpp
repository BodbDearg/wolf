//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdarg>
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

struct Token;

WC_AST_BEGIN_NAMESPACE

class ASTNodeVisitor;
class Func;
class Module;
class ParseCtx;
class Scope;

/* Macro for allocating an AST tree node */
#define WC_NEW_AST_NODE(parseCtx, NodeType, ...)\
    new(parseCtx.getAlloc().allocSizeOf<NodeType>()) NodeType(__VA_ARGS__)

/* Abstract base class for all AST nodes. */
class ASTNode {
public:
    ASTNode();
    
    virtual ~ASTNode();
    
    /* Accept an AST node visitor */
    virtual void accept(ASTNodeVisitor & visitor) const = 0;
    
    /* Return the token that the AST node starts at. */
    virtual const Token & getStartToken() const = 0;
    
    /* Return the token that the AST node ends at. */
    virtual const Token & getEndToken() const = 0;
    
    /**
     * Return a token which is 1 token past the last token of the AST node. 
     * One exception however, if the end token happens to be EOF then the EOF token will just be returned.
     */
    const Token & getPastEndToken() const;
    
    /* Figure out what the parent scope of this node is. */
    Scope * getParentScope();
    
    /* Figure out what the parent scope of this node is (const version). */
    const Scope * getParentScope() const;
    
    /* Figure out what the parent function of this node is. */
    Func * getParentFunc();
    
    /* Figure out what the parent function of this node is (const version). */
    const Func * getParentFunc() const;
    
    /* Figure out what the parent module of this node is. */
    Module * getParentModule();
    
    /* Figure out what the parent module of this node is (const version). */
    const Module * getParentModule() const;
    
    /* Get the first parent node of a certain type */
    template <class T>
    inline T * firstParentOfType() {
        ASTNode * parent = mParent;
        
        while (parent) {
            if (T * nodeCast = dynamic_cast<T*>(parent)) {
                return nodeCast;
            }
            
            parent = parent->mParent;
        }
        
        return nullptr;
    }
    
    /* Get the first parent node of a certain type (const version). */
    template <class T>
    inline const T * firstParentOfType() const {
        const ASTNode * parent = mParent;
        
        while (parent) {
            if (const T * nodeCast = dynamic_cast<const T*>(parent)) {
                return nodeCast;
            }
            
            parent = parent->mParent;
        }
        
        return nullptr;
    }
    
    /* The parent of this AST node. This should be set by the parent itself in the constructor. */
    ASTNode * mParent;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
