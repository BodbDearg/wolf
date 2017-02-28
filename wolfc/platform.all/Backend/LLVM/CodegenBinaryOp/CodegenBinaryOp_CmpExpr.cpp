//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenBinaryOp_CmpExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PrimitiveDataTypes.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/* This macro saves a lot of code */
#define WC_IMPL_BASIC_CMP_OP(VisitorClass,\
                             DataTypeName,\
                             CmpExprCreateFunc,\
                             CmpLabel)\
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
        llvm::Value * cmpResult = mCG.mCtx.mIRBuilder.CmpExprCreateFunc(\
            mLeftVal.mLLVMVal,\
            mRightVal.mLLVMVal,\
            #DataTypeName ":" CmpLabel ":Result"\
        );\
        \
        WC_ASSERT(cmpResult);\
        pushOpResult(cmpResult, false, compiledType);\
    }

//-----------------------------------------------------------------------------
// CodegenCmpBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpBinaryOp::CodegenCmpBinaryOp(Codegen & cg,
                                       const AST::ASTNode & leftExpr,
                                       const AST::ASTNode & rightExpr,
                                       const char * opSymbol,
                                       const char * opName,
                                       bool storeResultOnLeft)
:
    CodegenBinaryOp(cg,
                    leftExpr,
                    rightExpr,
                    opSymbol,
                    opName,
                    storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

//-----------------------------------------------------------------------------
// CodegenCmpEQBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpEQBinaryOp::CodegenCmpEQBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr,
                                           bool storeResultOnLeft)
:
    CodegenCmpBinaryOp(cg,
                       leftExpr,
                       rightExpr,
                       "==",
                       "equals",
                       storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, Bool, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, Int128, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, Int16, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, Int32, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, Int64, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, Int8, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, UInt128, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, UInt16, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, UInt32, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, UInt64, CreateICmpEQ, "CmpEQ")
WC_IMPL_BASIC_CMP_OP(CodegenCmpEQBinaryOp, UInt8, CreateICmpEQ, "CmpEQ")

//-----------------------------------------------------------------------------
// CodegenCmpNEBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpNEBinaryOp::CodegenCmpNEBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr,
                                           bool storeResultOnLeft)
:
    CodegenCmpBinaryOp(cg,
                       leftExpr,
                       rightExpr,
                       "!=",
                       "not equals",
                       storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, Bool, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, Int128, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, Int16, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, Int32, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, Int64, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, Int8, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, UInt128, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, UInt16, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, UInt32, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, UInt64, CreateICmpNE, "CmpNE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpNEBinaryOp, UInt8, CreateICmpNE, "CmpNE")

//-----------------------------------------------------------------------------
// CodegenCmpLTBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpLTBinaryOp::CodegenCmpLTBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr,
                                           bool storeResultOnLeft)
:
    CodegenCmpBinaryOp(cg,
                       leftExpr,
                       rightExpr,
                       "<",
                       "less than",
                       storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, Int128, CreateICmpSLT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, Int16, CreateICmpSLT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, Int32, CreateICmpSLT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, Int64, CreateICmpSLT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, Int8, CreateICmpSLT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, UInt128, CreateICmpULT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, UInt16, CreateICmpULT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, UInt32, CreateICmpULT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, UInt64, CreateICmpULT, "CmpLT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLTBinaryOp, UInt8, CreateICmpULT, "CmpLT")

//-----------------------------------------------------------------------------
// CodegenCmpLEBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpLEBinaryOp::CodegenCmpLEBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr,
                                           bool storeResultOnLeft)
:
    CodegenCmpBinaryOp(cg,
                       leftExpr,
                       rightExpr,
                       "<=",
                       "less than or equal to",
                       storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, Int128, CreateICmpSLE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, Int16, CreateICmpSLE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, Int32, CreateICmpSLE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, Int64, CreateICmpSLE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, Int8, CreateICmpSLE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, UInt128, CreateICmpULE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, UInt16, CreateICmpULE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, UInt32, CreateICmpULE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, UInt64, CreateICmpULE, "CmpLE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpLEBinaryOp, UInt8, CreateICmpULE, "CmpLE")

//-----------------------------------------------------------------------------
// CodegenCmpGTBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpGTBinaryOp::CodegenCmpGTBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr,
                                           bool storeResultOnLeft)
:
    CodegenCmpBinaryOp(cg,
                       leftExpr,
                       rightExpr,
                       ">",
                       "greater than",
                       storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, Int128, CreateICmpSGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, Int16, CreateICmpSGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, Int32, CreateICmpSGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, Int64, CreateICmpSGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, Int8, CreateICmpSGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, UInt128, CreateICmpUGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, UInt16, CreateICmpUGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, UInt32, CreateICmpUGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, UInt64, CreateICmpUGT, "CmpGT")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGTBinaryOp, UInt8, CreateICmpUGT, "CmpGT")

//-----------------------------------------------------------------------------
// CodegenCmpGEBinaryOp
//-----------------------------------------------------------------------------
CodegenCmpGEBinaryOp::CodegenCmpGEBinaryOp(Codegen & cg,
                                           const AST::ASTNode & leftExpr,
                                           const AST::ASTNode & rightExpr,
                                           bool storeResultOnLeft)
:
    CodegenCmpBinaryOp(cg,
                       leftExpr,
                       rightExpr,
                       ">=",
                       "greater than or equal to",
                       storeResultOnLeft)
{
    WC_EMPTY_FUNC_BODY();
}

WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, Int128, CreateICmpSGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, Int16, CreateICmpSGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, Int32, CreateICmpSGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, Int64, CreateICmpSGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, Int8, CreateICmpSGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, UInt128, CreateICmpUGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, UInt16, CreateICmpUGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, UInt32, CreateICmpUGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, UInt64, CreateICmpUGE, "CmpGE")
WC_IMPL_BASIC_CMP_OP(CodegenCmpGEBinaryOp, UInt8, CreateICmpUGE, "CmpGE")

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
