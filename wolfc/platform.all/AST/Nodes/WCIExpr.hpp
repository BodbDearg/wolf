#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Constant;
    class Value;
}

WC_BEGIN_NAMESPACE

struct CodegenCtx;
class DataType;

WC_AST_BEGIN_NAMESPACE

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

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
