#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

class Scope;
struct Token;

/* Abstract base class for all AST nodes. */
class ASTNode {
public:
    /**
     * Emit a formatted error message followed by a newline to stderr for a parse error at the given token.
     * Line and column information from the given token are added also.
     */
    static void parseError(const Token & srcToken, const char * msg, ...);
    
    ASTNode();
    
    virtual ~ASTNode();
    
    /* Return the token that the AST node starts at. */
    virtual const Token & getStartToken() const = 0;
    
    /* Return the token that the AST node ends at. */
    virtual const Token & getEndToken() const = 0;
    
    /* Figure out what the parent scope AST node of this node is. */
    Scope * getParentScope();
    
    /* Figure out what the parent scope AST node of this node is (const version). */
    const Scope * getParentScope() const;
    
    /* Get the first parent node of a certain type */
    template <class T>
    T * firstParentOfType() {
        if (mParent) {
            T * nodeCast = dynamic_cast<T*>(mParent);
            
            if (nodeCast) {
                return nodeCast;
            }
            
            return mParent->firstParentOfType<T>();
        }
        
        return nullptr;
    }
    
    /* Get the first parent node of a certain type (const version). */
    template <class T>
    const T * firstParentOfType() const {
        if (mParent) {
            T * nodeCast = dynamic_cast<const T*>(mParent);
            
            if (nodeCast) {
                return nodeCast;
            }
            
            return mParent->firstParentOfType<T>();
        }
        
        return nullptr;
    }
    
    /**
     * Emit a formatted compile error message followed by a newline to stderr for a compile error.
     * The line and column information for this node are output also.
     */
    void compileError(const char * msg, ...) const;
    
    /* The parent of this AST node. This should be set by the parent itself in the constructor. */
    ASTNode * mParent;
};

WC_END_NAMESPACE
