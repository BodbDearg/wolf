//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../Value.hpp"
#include "Assert.hpp"
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
 * Base class for most unary operations that operate on an expression of a specific data type.
 * The default implementation of each visitor function says that the op is not supported for the data type.
 */
class CodegenBasicUnaryOp : public DataTypeVisitor {
public:
    CodegenBasicUnaryOp(Codegen & cg,
                        const AST::ASTNode & expr,
                        const char * opSymbol,
                        const char * opName,
                        bool storeResultInExpr);
    
    /**
     * Kicks off the codegen for the unary op.
     * Evaluates the operand of the expression before calling the visitor functions 
     * with the value needed (if everything is ok).
     */
    virtual void codegen();
    
    /* DataType visitor functions */
    virtual void visit(const AnyDataType & dataType) override;
    virtual void visit(const ArrayDataType & dataType) override;
    virtual void visit(const BoolDataType & dataType) override;
    virtual void visit(const FuncDataType & dataType) override;
    virtual void visit(const Int128DataType & dataType) override;
    virtual void visit(const Int16DataType & dataType) override;
    virtual void visit(const Int32DataType & dataType) override;
    virtual void visit(const Int64DataType & dataType) override;
    virtual void visit(const Int8DataType & dataType) override;
    virtual void visit(const NullptrDataType & dataType) override;
    virtual void visit(const PtrDataType & dataType) override;
    virtual void visit(const StrDataType & dataType) override;
    virtual void visit(const UInt128DataType & dataType) override;
    virtual void visit(const UInt16DataType & dataType) override;
    virtual void visit(const UInt32DataType & dataType) override;
    virtual void visit(const UInt64DataType & dataType) override;
    virtual void visit(const UInt8DataType & dataType) override;
    virtual void visit(const UndefinedDataType & dataType) override;
    virtual void visit(const VoidDataType & dataType) override;
    
protected:
    /* Issue a compile error that a unary operation is not supported! */
    void issueUnaryOpNotSupportedError();
    
    /**
     * Perform automatic type casting required for the expression value.
     * The default implementation of this simply does nothing.
     */
    virtual void doAnyImplicitTypeCastingRequiredForExprVal();
    
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

/* This macro saves a lot of code */
#define WC_IMPL_BASIC_UNARY_OP(VisitorClass, DataTypeName, OpCreateFunc)\
    void VisitorClass::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        WC_ASSERT(mExprVal.mLLVMVal);\
        \
        llvm::Value * resultVal = mCG.mCtx.mIRBuilder.OpCreateFunc(\
            mExprVal.mLLVMVal,\
            #DataTypeName ":" #OpCreateFunc ":Result"\
        );\
        \
        WC_ASSERT(resultVal);\
        pushOpResult(resultVal);\
    }

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
