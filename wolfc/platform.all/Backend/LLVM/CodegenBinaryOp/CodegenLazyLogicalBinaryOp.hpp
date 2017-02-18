#pragma once

#include "Macros.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class Codegen;

/**
 * Class that generates most of the code for a lazy logical binary operation on boolean operands.
 * Used for logical 'and' and 'or' operators.
 */
class CodegenLazyLogicalBinaryOp {
public:
    /* Which type of lazy logical operation, 'and' or 'or' */
    enum class OpType {
        kAnd,
        kOr
    };
    
    CodegenLazyLogicalBinaryOp(Codegen & cg,
                               const AST::ASTNode & leftExpr,
                               const AST::ASTNode & rightExpr,
                               OpType opType);
    
    virtual ~CodegenLazyLogicalBinaryOp() = default;
    
    /* Does the codegen for the lazy logical binary binary op. */
    virtual void codegen();
    
protected:
    /* Various vars needed as input to the code generator */
    Codegen &               mCG;
    const AST::ASTNode &    mLeftExpr;
    const AST::ASTNode &    mRightExpr;
    OpType                  mOpType;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
