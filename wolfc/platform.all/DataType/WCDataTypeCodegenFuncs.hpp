#pragma once

#include "WCMacros.hpp"

#warning FIXME - Codegen
#if 0
namespace llvm {
    class Constant;
    class Value;
}
#endif

WC_BEGIN_NAMESPACE

class DataType;

#warning FIXME - Codegen
#if 0
struct CodegenCtx;
#endif

namespace AST {
    class ASTNode;
}

#warning FIXME - Codegen
#if 0
/** 
 * Typedef for a 'DataType' member function which codegens a runtime binary op.
 * For example, a function that generates code for an 'add' operation.
 */
typedef llvm::Value * (DataType::*DTCodegenBinaryOpFunc)(
    CodegenCtx & cgCtx,
    AST::ASTNode & callingNode,
    llvm::Value & leftVal,
    DataType & rightTy,
    llvm::Value & rightVal
);

/** 
 * Typedef for a 'DataType' member function which codegens a compile time constant binary op.
 * For example, a function that generates code for a constant 'add' operation.
 */
typedef llvm::Constant * (DataType::*DTCodegenConstBinaryOpFunc)(
    AST::ASTNode & callingNode,
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
    AST::ASTNode & callingNode,
    llvm::Value & val
);

/**
 * Typedef for a 'DataType' member function which codegens a compile time constant unary op.
 * For example, a function that generates code for a constant 'negate' operation.
 */
typedef llvm::Constant * (DataType::*DTCodegenConstUnaryOpFunc)(
    AST::ASTNode & callingNode,
    llvm::Constant & val
);
#endif

WC_END_NAMESPACE
