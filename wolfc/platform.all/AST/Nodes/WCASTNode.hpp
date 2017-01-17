#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdarg>
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

struct Token;

WC_AST_BEGIN_NAMESPACE

class Func;
class Module;
class Scope;

/* Macro for allocating an AST tree node */
#define WC_NEW_AST_NODE(parseCtx, NodeType, ...)\
    new(parseCtx.alloc.allocSizeOf<NodeType>()) NodeType(__VA_ARGS__)

/* Abstract base class for all AST nodes. */
class ASTNode {
public:
    ASTNode();
    
    virtual ~ASTNode();
    
    /* Return the token that the AST node starts at. */
    virtual const Token & getStartToken() const = 0;
    
    /* Return the token that the AST node ends at. */
    virtual const Token & getEndToken() const = 0;
    
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
    
#warning FIXME - Codegen
#if 0
    /**
     * Emit a formatted compile error message followed by a newline to stderr for a compile error.
     * The line and column information for this node are output also.
     */
    void compileError(const char * msg, ...) const;
    
    /* Makeup an LLVM label for something and include the line number and column number info of the given token */
    std::string makeLLVMLabelForTok(const char * labelText, const Token & token);
#endif
    
    /* The parent of this AST node. This should be set by the parent itself in the constructor. */
    ASTNode * mParent;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
