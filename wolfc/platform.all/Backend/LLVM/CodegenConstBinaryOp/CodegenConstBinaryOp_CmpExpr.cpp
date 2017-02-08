#include "CodegenConstBinaryOp_CmpExpr.hpp"

#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "Assert.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "DataType/Primitives/BoolDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

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

void CodegenConstCmpBinaryOp::codegenIntCompare(llvm::ICmpInst::Predicate predicate) {
    // Codegen the datatype for type 'bool', this is the datatype for the operator result
    {
        const BoolDataType & boolDataType = PrimitiveDataTypes::getBoolDataType();
        boolDataType.accept(mCG.mCodegenDataType);
    }
    
    CompiledDataType compiledType = mCG.mCtx.popCompiledDataType();
    
    // Generate the compare op and push to the codegen context
    llvm::Constant * constant = llvm::ConstantExpr::getICmp(predicate,
                                                            mLeftConst.mLLVMConst,
                                                            mRightConst.mLLVMConst);
    
    WC_ASSERT(constant);
    pushOpResult(constant, compiledType);
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

void CodegenConstCmpEQBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_EQ);
}

void CodegenConstCmpEQBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_EQ);
}

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

void CodegenConstCmpNEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_NE);
}

void CodegenConstCmpNEBinaryOp::visit(const BoolDataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_NE);
}

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

void CodegenConstCmpLTBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SLT);
}

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

void CodegenConstCmpLEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SLE);
}

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

void CodegenConstCmpGTBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SGT);
}

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

void CodegenConstCmpGEBinaryOp::visit(const Int64DataType & dataType) {
    WC_UNUSED_PARAM(dataType);
    codegenIntCompare(llvm::ICmpInst::Predicate::ICMP_SGE);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
