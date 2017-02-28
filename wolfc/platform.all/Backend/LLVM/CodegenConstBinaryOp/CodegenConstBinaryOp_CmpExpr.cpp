#include "CodegenConstBinaryOp_CmpExpr.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* This macro saves a lot of code */
#define WC_IMPL_BASIC_CONST_CMP_OP(VisitorClass,\
                                   DataTypeName,\
                                   ConstCmpExprGetterFunc,\
                                   CmpPredicate)\
    \
    void VisitorClass::visit(const DataTypeName##DataType & dataType) {\
        WC_UNUSED_PARAM(dataType);\
        \
        /* Codegen the datatype for type 'bool', this is the datatype for the operator result */\
        {\
            const BoolDataType & boolDataType = PrimitiveDataTypes::getBoolDataType();\
            boolDataType.accept(mCG.mCodegenDataType);\
        }\
        \
        CompiledDataType compiledType = mCG.mCtx.popCompiledDataType();\
        \
        /* Generate the op result and push it */\
        llvm::Constant * cmpResult = llvm::ConstantExpr::ConstCmpExprGetterFunc(\
            CmpPredicate,\
            mLeftConst.mLLVMConst,\
            mRightConst.mLLVMConst\
        );\
        \
        WC_ASSERT(cmpResult);\
        pushOpResult(cmpResult, compiledType);\
    }

//-----------------------------------------------------------------------------
// CodegenConstCmpBinaryOp
//-----------------------------------------------------------------------------
CodegenConstCmpBinaryOp::CodegenConstCmpBinaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & leftExpr,
                                                 const AST::ASTNode & rightExpr,
                                                 const char * opSymbol,
                                                 const char * opName)
:
    CodegenConstBinaryOp(cg, leftExpr, rightExpr, opSymbol, opName)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// CodegenConstCmpEQBinaryOp
//-----------------------------------------------------------------------------
CodegenConstCmpEQBinaryOp::CodegenConstCmpEQBinaryOp(ConstCodegen & cg,
                                                     const AST::ASTNode & leftExpr,
                                                     const AST::ASTNode & rightExpr)
:
    CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "==", "equals")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, Bool, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, Int128, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, Int16, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, Int32, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, Int64, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, Int8, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, UInt128, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, UInt16, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, UInt32, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, UInt64, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpEQBinaryOp, UInt8, getICmp, llvm::ICmpInst::Predicate::ICMP_EQ)

//-----------------------------------------------------------------------------
// CodegenConstCmpNEBinaryOp
//-----------------------------------------------------------------------------
CodegenConstCmpNEBinaryOp::CodegenConstCmpNEBinaryOp(ConstCodegen & cg,
                                                     const AST::ASTNode & leftExpr,
                                                     const AST::ASTNode & rightExpr)
:
    CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "!=", "not equals")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, Bool, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, Int128, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, Int16, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, Int32, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, Int64, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, Int8, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, UInt128, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, UInt16, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, UInt32, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, UInt64, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpNEBinaryOp, UInt8, getICmp, llvm::ICmpInst::Predicate::ICMP_NE)

//-----------------------------------------------------------------------------
// CodegenConstCmpLTBinaryOp
//-----------------------------------------------------------------------------
CodegenConstCmpLTBinaryOp::CodegenConstCmpLTBinaryOp(ConstCodegen & cg,
                                                     const AST::ASTNode & leftExpr,
                                                     const AST::ASTNode & rightExpr)
:
    CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "<", "less than")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, Int128, getICmp, llvm::ICmpInst::Predicate::ICMP_SLT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, Int16, getICmp, llvm::ICmpInst::Predicate::ICMP_SLT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, Int32, getICmp, llvm::ICmpInst::Predicate::ICMP_SLT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, Int64, getICmp, llvm::ICmpInst::Predicate::ICMP_SLT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, Int8, getICmp, llvm::ICmpInst::Predicate::ICMP_SLT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, UInt128, getICmp, llvm::ICmpInst::Predicate::ICMP_ULT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, UInt16, getICmp, llvm::ICmpInst::Predicate::ICMP_ULT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, UInt32, getICmp, llvm::ICmpInst::Predicate::ICMP_ULT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, UInt64, getICmp, llvm::ICmpInst::Predicate::ICMP_ULT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLTBinaryOp, UInt8, getICmp, llvm::ICmpInst::Predicate::ICMP_ULT)

//-----------------------------------------------------------------------------
// CodegenConstCmpLEBinaryOp
//-----------------------------------------------------------------------------
CodegenConstCmpLEBinaryOp::CodegenConstCmpLEBinaryOp(ConstCodegen & cg,
                                                     const AST::ASTNode & leftExpr,
                                                     const AST::ASTNode & rightExpr)
:
    CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, "<=", "less than or equal to")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, Int128, getICmp, llvm::ICmpInst::Predicate::ICMP_SLE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, Int16, getICmp, llvm::ICmpInst::Predicate::ICMP_SLE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, Int32, getICmp, llvm::ICmpInst::Predicate::ICMP_SLE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, Int64, getICmp, llvm::ICmpInst::Predicate::ICMP_SLE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, Int8, getICmp, llvm::ICmpInst::Predicate::ICMP_SLE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, UInt128, getICmp, llvm::ICmpInst::Predicate::ICMP_ULE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, UInt16, getICmp, llvm::ICmpInst::Predicate::ICMP_ULE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, UInt32, getICmp, llvm::ICmpInst::Predicate::ICMP_ULE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, UInt64, getICmp, llvm::ICmpInst::Predicate::ICMP_ULE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpLEBinaryOp, UInt8, getICmp, llvm::ICmpInst::Predicate::ICMP_ULE)

//-----------------------------------------------------------------------------
// CodegenConstCmpGTBinaryOp
//-----------------------------------------------------------------------------
CodegenConstCmpGTBinaryOp::CodegenConstCmpGTBinaryOp(ConstCodegen & cg,
                                                     const AST::ASTNode & leftExpr, 
                                                     const AST::ASTNode & rightExpr)
:
    CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, ">", "greater than")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, Int128, getICmp, llvm::ICmpInst::Predicate::ICMP_SGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, Int16, getICmp, llvm::ICmpInst::Predicate::ICMP_SGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, Int32, getICmp, llvm::ICmpInst::Predicate::ICMP_SGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, Int64, getICmp, llvm::ICmpInst::Predicate::ICMP_SGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, Int8, getICmp, llvm::ICmpInst::Predicate::ICMP_SGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, UInt128, getICmp, llvm::ICmpInst::Predicate::ICMP_UGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, UInt16, getICmp, llvm::ICmpInst::Predicate::ICMP_UGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, UInt32, getICmp, llvm::ICmpInst::Predicate::ICMP_UGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, UInt64, getICmp, llvm::ICmpInst::Predicate::ICMP_UGT)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGTBinaryOp, UInt8, getICmp, llvm::ICmpInst::Predicate::ICMP_UGT)

//-----------------------------------------------------------------------------
// CodegenConstCmpGEBinaryOp
//-----------------------------------------------------------------------------
CodegenConstCmpGEBinaryOp::CodegenConstCmpGEBinaryOp(ConstCodegen & cg,
                                                     const AST::ASTNode & leftExpr,
                                                     const AST::ASTNode & rightExpr)
:
    CodegenConstCmpBinaryOp(cg, leftExpr, rightExpr, ">=", "greater than or equal to")
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, Int128, getICmp, llvm::ICmpInst::Predicate::ICMP_SGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, Int16, getICmp, llvm::ICmpInst::Predicate::ICMP_SGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, Int32, getICmp, llvm::ICmpInst::Predicate::ICMP_SGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, Int64, getICmp, llvm::ICmpInst::Predicate::ICMP_SGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, Int8, getICmp, llvm::ICmpInst::Predicate::ICMP_SGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, UInt128, getICmp, llvm::ICmpInst::Predicate::ICMP_UGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, UInt16, getICmp, llvm::ICmpInst::Predicate::ICMP_UGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, UInt32, getICmp, llvm::ICmpInst::Predicate::ICMP_UGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, UInt64, getICmp, llvm::ICmpInst::Predicate::ICMP_UGE)
WC_IMPL_BASIC_CONST_CMP_OP(CodegenConstCmpGEBinaryOp, UInt8, getICmp, llvm::ICmpInst::Predicate::ICMP_UGE)

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
