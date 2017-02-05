#include "CodegenBinaryOp_CmpExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/BoolDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

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

void CodegenCmpBinaryOp::codegenICmp(llvm::CmpInst::Predicate predicate,
                                     const char * instructionLabel)
{
    // Codegen the datatype for type 'bool'
    {
        const BoolDataType & boolDataType = PrimitiveDataTypes::getBoolDataType();
        boolDataType.accept(mCG.mCodegenDataType);
    }
    
    CompiledDataType compiledType = mCG.mCtx.popCompiledDataType();
    
    // Create the compare instruction and save the result
    llvm::Value * cmpResult = mCG.mCtx.mIRBuilder.CreateICmp(predicate,
                                                             mLeftVal.mLLVMVal,
                                                             mRightVal.mLLVMVal,
                                                             instructionLabel);
    
    WC_ASSERT(cmpResult);
    pushOpResult(cmpResult, false, compiledType);
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
    
void CodegenCmpEQBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_EQ, "Bool:CmpEQ:Result");
}

void CodegenCmpEQBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_EQ, "Int64:CmpEQ:Result");
}

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

void CodegenCmpNEBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_NE, "Bool:CmpNE:Result");
}

void CodegenCmpNEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_NE, "Int64:CmpNE:Result");
}

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

void CodegenCmpLTBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_SLT, "Int64:CmpLT:Result");
}

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

void CodegenCmpLEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_SLE, "Int64:CmpLE:Result");
}

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

void CodegenCmpGTBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_SGT, "Int64:CmpGT:Result");
}

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

void CodegenCmpGEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenICmp(llvm::CmpInst::Predicate::ICMP_SGE, "Int64:CmpGE:Result");
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
