#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

class Scope;
struct Token;

/* Abstract base class for all AST nodes. */
class ASTNode {
public:
    /* Emit a formatted error message followed by a newline to stderr. */
    static void error(const char * msg, ...);
    
    /**
     * Emit a formatted error message followed by a newline to stderr.
     * Line and column information from the given token are added also. 
     */
    static void error(const Token & srcToken, const char * msg, ...);
    
    ASTNode();
    
    virtual ~ASTNode();
    
    /* Return the token that the AST node starts at. */
    virtual const Token & getStartToken() const = 0;
    
    /* Return the token that the AST node ends at. */
    virtual const Token & getEndToken() const = 0;
    
    /* Figure out what the parent scope AST node of this node is. */
    Scope * getParentScope();
    
    /* The parent of this AST node. This should be set by the parent itself in the constructor. */
    ASTNode * mParent;
};

WC_END_NAMESPACE
