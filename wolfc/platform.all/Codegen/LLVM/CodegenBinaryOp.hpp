#pragma once

#include "DataType/DataTypeVisitor.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <functional>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Value;
}

WC_BEGIN_NAMESPACE

class DataType;

namespace AST {
    class ASTNode;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class Codegen;

/**
 * Class that generates code for a binary operation for a specific data type.
 * The default implementation of each visitor function says that the op is
 * not supported for the data type.
 */
class CodegenBinaryOp : public DataTypeVisitor {
public:
    CodegenBinaryOp(Codegen & cg,
                    const AST::ASTNode & leftExpr,
                    const AST::ASTNode & rightExpr,
                    const char * opSymbol,
                    const char * opName);
    
    /**
     * Kicks off the codegen for the binary op.
     * Evaluates the left and right sides of the expression and makes sure both types match etc.
     * before calling the visitor functions with the values needed (if everything is ok).
     */
    void codegen();
    
    /* DataType visitor functions */
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const UnknownArrayDataType & dataType) override;
    virtual void visit(const UnknownDataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
protected:
    /* Issue a compile error that a binary operation is not supported! */
    void issueBinaryOpNotSupportedError();
    
    /* Save the operator result in the codegen context */
    void pushOpResult(llvm::Value * result);
    
    /* Various vars needed as input to the code generator */
    Codegen &               mCG;
    const AST::ASTNode &    mLeftExpr;
    const AST::ASTNode &    mRightExpr;
    const char *            mOpSymbol;
    const char *            mOpName;
    
    /* These are generated and cached by the codegen() function */
    const DataType *    mLeftType = nullptr;
    const DataType *    mRightType = nullptr;
    llvm::Value *       mLeftVal = nullptr;
    llvm::Value *       mRightVal = nullptr;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
