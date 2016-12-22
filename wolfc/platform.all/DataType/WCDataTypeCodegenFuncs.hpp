#pragma once

#include "WCMacros.hpp"

namespace llvm {
    class Constant;
    class Value;
}

WC_BEGIN_NAMESPACE

class ASTNode;
class DataType;
struct CodegenCtx;

/** 
 * Typedef for a 'DataType' member function which codegens a runtime binary op.
 * For example, a function that generates code for an 'add' operation.
 */
typedef llvm::Value * (DataType::*DTCodegenBinaryOpFunc)(
    CodegenCtx & cgCtx,
    ASTNode & callingNode,
    llvm::Value & leftVal,
    DataType & rightTy,
    llvm::Value & rightVal
);

/** 
 * Typedef for a 'DataType' member function which codegens a compile time constant binary op.
 * For example, a function that generates code for a constant 'add' operation.
 */
typedef llvm::Constant * (DataType::*DTCodegenConstBinaryOpFunc)(
    ASTNode & callingNode,
    llvm::Constant & leftVal,
    DataType & rightTy,
    llvm::Constant & rightVal
);

WC_END_NAMESPACE
