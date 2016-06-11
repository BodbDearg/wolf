#pragma once

#include "WCMacros.hpp"

namespace llvm {
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
    virtual bool isLValue() const = 0;
    
    /* Return the data type that this expression results in after evaluation. */
    virtual const DataType & dataType() const = 0;
    
    /**
     * Codegen the llvm value that represents the address of this expression. 
     * Note: this is only possible for lvalues! For rvalues this should return nullptr.
     */
    virtual llvm::Value * codegenAddrOf(CodegenCtx & cgCtx) = 0;
    
    /**
     * Generate the llvm code to evaluate this expression.
     * The expression result is returned as an llvm value, or nullptr if there was a compile error.
     */
    virtual llvm::Value * codegenExprEval(CodegenCtx & cgCtx) = 0;
};

WC_END_NAMESPACE
