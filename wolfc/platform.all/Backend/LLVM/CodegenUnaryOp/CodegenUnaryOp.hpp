#pragma once

#include "../Value.hpp"
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

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class Codegen;

/**
 * Class that generates code for a unary operation for a specific data type.
 * The default implementation of each visitor function says that the op is
 * not supported for the data type.
 */
class CodegenUnaryOp : public DataTypeVisitor {
public:
    CodegenUnaryOp(Codegen & cg,
                   const AST::ASTNode & expr,
                   const char * opSymbol,
                   const char * opName,
                   bool storeResultInExpr);
    
    /**
     * Kicks off the codegen for the unary op.
     * Evaluates the operand of the expression before calling the visitor functions 
     * with the value needed (if everything is ok).
     */
    void codegen();
    
    /* DataType visitor functions */
    virtual void visit(const ArrayBadSizeDataType & dataType) override;
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const ArrayUnevalSizeDataType & dataType) override;    
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const FuncDataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const InvalidDataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
protected:
    /* Issue a compile error that a unary operation is not supported! */
    void issueUnaryOpNotSupportedError();
    
    /**
     * Save the operator result in the codegen context. If the compiled type of the result 
     * is not specified then we use the compiled type of the operand. If the 'requiresLoad'
     * flag is not specified then it is defaulted to false.
     */
    void pushOpResult(llvm::Value * result);
    void pushOpResult(llvm::Value * result, bool requiresLoad);
    void pushOpResult(llvm::Value * result, bool requiresLoad, const CompiledDataType & resultType);
    
    /* Various vars needed as input to the code generator */
    Codegen &               mCG;
    const AST::ASTNode &    mExpr;
    const char *            mOpSymbol;
    const char *            mOpName;
    bool                    mStoreResultInExpr;
    
    /* This is generated and cached by the codegen() function */
    Value mExprVal;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
