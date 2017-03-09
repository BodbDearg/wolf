//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../Constant.hpp"
#include "Assert.hpp"
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

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class ConstCodegen;

/**
 * Base class for most compile time unary operations that operate on an expression of a specific data type.
 * The default implementation of each visitor function says that the op is not supported for the data type.
 */
class CodegenBasicConstUnaryOp : public DataTypeVisitor {
public:
    CodegenBasicConstUnaryOp(ConstCodegen & cg,
                             const AST::ASTNode & expr,
                             const char * opSymbol,
                             const char * opName);
    
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
     * Perform automatic type casting required for the expression constant.
     * The default implementation of this simply does nothing.
     */
    virtual void doAnyImplicitTypeCastingRequiredForExprConst();
    
    /**
     * Save the operator result in the codegen context. If the compiled type of the result 
     * is not specified then we use the compiled type of the operand.
     */
    void pushOpResult(llvm::Constant * result);
    void pushOpResult(llvm::Constant * result, const CompiledDataType & resultType);
    
    /* Various vars needed as input to the code generator */
    ConstCodegen &          mCG;
    const AST::ASTNode &    mExpr;
    const char *            mOpSymbol;
    const char *            mOpName;
    
    /* This are generated and cached by the codegen() function */
    Constant mExprConst;
};

/* This macro saves a lot of code */
#define WC_IMPL_BASIC_CONST_UNARY_OP(VisitorClass, DataTypeName, ConstExprGetterFunc)\
    void VisitorClass::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        llvm::Constant * resultConst = llvm::ConstantExpr::ConstExprGetterFunc(mExprConst.mLLVMConst);\
        WC_ASSERT(resultConst);\
        pushOpResult(resultConst);\
    }

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
