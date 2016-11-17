#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Constant;
    class Value;
}

WC_BEGIN_NAMESPACE

class CodegenCtx;
class DataType;

/* An interface that all expressions must implement. */
class IExpr {
public:
    virtual ~IExpr() = default;
    
    /**
     * Tell if this expression evaluates to an lvalue. lvalues are values that can be asssigned to.
     * See lvalues versus rvalues: https://msdn.microsoft.com/en-us/library/f90831hc.aspx
     */
    virtual bool isLValue() = 0;
    
    /* Tells if it's possible for this expression to be evaluated as a constant */
    virtual bool isConstExpr() = 0;
    
    /* Return the data type that this expression results in after evaluation. */
    virtual DataType & dataType() = 0;
    
    /**
     * If true then the expression requires a storage area to hold since it is
     * not a simple primitive that can be held in a register.
     * This will be true for arrays, structs and other aggregate types.
     *
     * For expressions that require storage they have one of two sources to 
     * get their storage area from:
     *
     *  (1) Create it themselves on demand (usually on the stack) as the expression
     *      is evaluated.
     *  (2) Be assigned it by a variable declaration which has already reserved the
     *      space to hold the variable.
     */
    virtual bool requiresStorage() const;
    
    /* Return the storage area of the expression, if any. */
    virtual llvm::Value * getStorage() const;
    
    /**
     * Set the storage area of the expression. 
     * This should only be implemented for types where 'requiresStorage' is true.
     * Variable declarations can use this to assign a storage area to complex types
     * like array literals and so forth.
     *
     * Once set, the storage area should not be re-set. 
     * This should be a once only operation!
     */
    virtual void setStorage(llvm::Value & storage);
    
    /**
     * Codegen the llvm value that represents the address of this expression. 
     * Note: this is only possible for lvalues! For rvalues this should return nullptr.
     */
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) = 0;
    
    /**
     * Generate the llvm code to evaluate this expression at runtime.
     * The expression result is returned as an llvm value, or nullptr if there was a compile error.
     */
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) = 0;
    
    /**
     * Generate the llvm code to evaluate this expression at compile time.
     * Note that not all expressions will support this and may return nullptr if not suported.
     */
    virtual llvm::Constant * codegenExprConstEval(CodegenCtx & cgCtx) = 0;
};

WC_END_NAMESPACE
