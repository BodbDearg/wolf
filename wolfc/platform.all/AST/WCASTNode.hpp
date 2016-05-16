#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE

struct Token;

/* Abstract base class for all AST nodes. */
class ASTNode {
public:
    ASTNode(ASTNode * parent);
    
    virtual ~ASTNode();
    
    /* Emit a formatted error message followed by a newline to stderr. */
    static void error(const char * msg, ...);
    
    /**
     * Emit a formatted error message followed by a newline to stderr.
     * Line and column information from the given token are added also. 
     */
    static void error(const Token & srcToken, const char * msg, ...);
    
    ASTNode * mParent;
};

WC_END_NAMESPACE
