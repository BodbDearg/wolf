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

/**
 * Typedef for a 'DataType' member function which codegens a runtime unary op.
 * For example, a function that generates code for an 'negate' or 'increment' operation.
 */
typedef llvm::Value * (DataType::*DTCodegenUnaryOpFunc)(
    CodegenCtx & cgCtx,
    ASTNode & callingNode,
    llvm::Value & val
);

/**
 * Typedef for a 'DataType' member function which codegens a compile time constant unary op.
 * For example, a function that generates code for a constant 'negate' operation.
 */
typedef llvm::Constant * (DataType::*DTCodegenConstUnaryOpFunc)(
    ASTNode & callingNode,
    llvm::Constant & val
);

WC_END_NAMESPACE
