#pragma once

#include "DataType/DataTypeVisitor.hpp"
#include "Value.hpp"

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

WC_LLVM_BACKEND_BEGIN_NAMESPACE

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
                    const char * opName,
                    bool storeResultOnLeft);
    
    /**
     * Kicks off the codegen for the binary op.
     * Evaluates the left and right sides of the expression and makes sure both types match etc.
     * before calling the visitor functions with the values needed (if everything is ok).
     */
    virtual void codegen();
    
    /* DataType visitor functions */
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const FuncDataType & dataType) override;
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const InvalidDataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
protected:
    /* Issue a compile error that a binary operation is not supported! */
    void issueBinaryOpNotSupportedError();
    
    /**
     * Save the operator result in the codegen context. If the compiled type of the result 
     * is not specified then we use the compiled type of left operand. If the 'requiresLoad'
     * flag is not specified then it is defaulted to false.
     */
    void pushOpResult(llvm::Value * result);
    
    void pushOpResult(llvm::Value * result,
                      bool requiresLoad);
    
    void pushOpResult(llvm::Value * result,
                      bool requiresLoad,
                      const CompiledDataType & resultType);
    
    /* Various vars needed as input to the code generator */
    Codegen &               mCG;
    const AST::ASTNode &    mLeftExpr;
    const AST::ASTNode &    mRightExpr;
    const char *            mOpSymbol;
    const char *            mOpName;
    bool                    mStoreResultOnLeft;
    
    /* These are generated and cached by the codegen() function */
    Value mLeftVal;
    Value mRightVal;
};

/* This macro saves a lot of code */
#define WC_IMPL_BASIC_BINARY_OP(VisitorClass, DataTypeName, OpCreateFunc)\
    void VisitorClass::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        pushOpResult(mCG.mCtx.mIRBuilder.OpCreateFunc(mLeftVal.mLLVMVal,\
                                                      mRightVal.mLLVMVal,\
                                                      #DataTypeName ":" #OpCreateFunc ":Result"));\
    }

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
