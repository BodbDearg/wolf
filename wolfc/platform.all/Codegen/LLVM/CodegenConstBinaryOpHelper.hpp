#pragma once

#include "DataType/DataTypeVisitor.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <functional>
WC_THIRD_PARTY_INCLUDES_END

namespace llvm {
    class Constant;
}

WC_BEGIN_NAMESPACE

class DataType;

namespace AST {
    class ASTNode;
}

WC_LLVM_CODEGEN_BEGIN_NAMESPACE

class ConstCodegen;

/**
 * Class that generates code for a const binary operation for a specific data type.
 * The default implementation of each visitor function says that the op is not 
 * supported for the data type.
 */
class CodegenConstBinaryOpHelper : public DataTypeVisitor {
public:
    CodegenConstBinaryOpHelper(ConstCodegen & cg,
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
    virtual void visit(const ArrayBadSizeDataType & dataType) override;
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const ArrayUnevalSizeDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const UnknownDataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
protected:
    /* Issue a compile error that a binary operation is not supported! */
    void issueBinaryOpNotSupportedError();
    
    /* Save the operator result in the codegen context */
    void pushOpResult(llvm::Constant * result);
    
    /* Various vars needed as input to the code generator */
    ConstCodegen &          mCG;
    const AST::ASTNode &    mLeftExpr;
    const AST::ASTNode &    mRightExpr;
    const char *            mOpSymbol;
    const char *            mOpName;
    
    /* These are generated and cached by the codegen() function */
    const DataType *    mLeftType = nullptr;
    const DataType *    mRightType = nullptr;
    llvm::Constant *    mLeftVal = nullptr;
    llvm::Constant *    mRightVal = nullptr;
};

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
