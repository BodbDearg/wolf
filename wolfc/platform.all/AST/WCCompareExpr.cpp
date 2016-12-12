#include "WCCompareExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"
#include "WCShiftExpr.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// CompareExpr
//-----------------------------------------------------------------------------

bool CompareExpr::peek(const Token * tokenPtr) {
    return ShiftExpr::peek(tokenPtr);
}

CompareExpr * CompareExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    ShiftExpr * leftExpr = ShiftExpr::parse(tokenPtr, alloc);
    WC_GUARD(leftExpr, nullptr);
    
    // See what tokens follow:
    if (tokenPtr[0].type == TokenType::kCmpEQ) {
        // '==' operator : skip this token
        ++tokenPtr;
        
        // Parse the right expression and return the AST node for the operation
        CompareExpr * rightExpr = CompareExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, CompareExprEQ, *leftExpr, *rightExpr);
    }
    else if (tokenPtr[0].type == TokenType::kCmpNE) {
        // '!=' operator : skip this token
        ++tokenPtr;
        
        // Parse the right expression and return the AST node for the operation
        CompareExpr * rightExpr = CompareExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, CompareExprNE, *leftExpr, *rightExpr);
    }
    else if (tokenPtr[0].type == TokenType::kCmpLT) {
        // '<' operator : skip this token
        ++tokenPtr;
        
        // Parse the right expression and return the AST node for the operation
        CompareExpr * rightExpr = CompareExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, CompareExprLT, *leftExpr, *rightExpr);
    }
    else if (tokenPtr[0].type == TokenType::kCmpLE) {
        // '<=' operator : skip this token
        ++tokenPtr;
        
        // Parse the right expression and return the AST node for the operation
        CompareExpr * rightExpr = CompareExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, CompareExprLE, *leftExpr, *rightExpr);
    }
    else if (tokenPtr[0].type == TokenType::kCmpGT) {
        // '>' operator : skip this token
        ++tokenPtr;
        
        // Parse the right expression and return the AST node for the operation
        CompareExpr * rightExpr = CompareExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, CompareExprGT, *leftExpr, *rightExpr);
    }
    else if (tokenPtr[0].type == TokenType::kCmpGE) {
        // '>=' operator : skip this token
        ++tokenPtr;
        
        // Parse the right expression and return the AST node for the operation
        CompareExpr * rightExpr = CompareExpr::parse(tokenPtr, alloc);
        WC_GUARD(rightExpr, nullptr);
        return WC_NEW_AST_NODE(alloc, CompareExprGE, *leftExpr, *rightExpr);
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, CompareExprNoOp, *leftExpr);
}

//-----------------------------------------------------------------------------
// CompareExprNoOp
//-----------------------------------------------------------------------------

CompareExprNoOp::CompareExprNoOp(ShiftExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & CompareExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & CompareExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool CompareExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool CompareExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & CompareExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * CompareExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * CompareExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * CompareExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// CompareExprTwoOps
//-----------------------------------------------------------------------------

CompareExprTwoOps::CompareExprTwoOps(ShiftExpr & leftExpr, CompareExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & CompareExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & CompareExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool CompareExprTwoOps::isLValue() {
    return false;
}

bool CompareExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & CompareExprTwoOps::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * CompareExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of relational operation result!");
    return nullptr;
}

bool CompareExprTwoOps::compileCheckBothExprsAreInt() const {
    const DataType & leftType = mLeftExpr.dataType();
    
    if (!leftType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Left type in expression must be 'int' for now and not '%s'!",
                     leftType.name().c_str());
        
        return false;
    }
    
    const DataType & rightType = mRightExpr.dataType();
    
    if (!rightType.equals(PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kInt))) {
        compileError("Right type in expression must be 'int' for now and not '%s'!",
                     rightType.name().c_str());
        
        return false;
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// CompareExprEQ
//-----------------------------------------------------------------------------

CompareExprEQ::CompareExprEQ(ShiftExpr & leftExpr, CompareExpr & rightExpr) : CompareExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CompareExprEQ::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpEQ(left, right, "CompareExprEQ_CmpOp");
}

llvm::Constant * CompareExprEQ::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_EQ, left, right);
}

//-----------------------------------------------------------------------------
// CompareExprNE
//-----------------------------------------------------------------------------

CompareExprNE::CompareExprNE(ShiftExpr & leftExpr, CompareExpr & rightExpr) : CompareExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CompareExprNE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpNE(left, right, "CompareExprNEQ_CmpOp");
}

llvm::Constant * CompareExprNE::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_NE, left, right);
}

//-----------------------------------------------------------------------------
// CompareExprLT
//-----------------------------------------------------------------------------

CompareExprLT::CompareExprLT(ShiftExpr & leftExpr, CompareExpr & rightExpr) : CompareExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CompareExprLT::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLT(left, right, "CompareExprLT_CmpOp");
}

llvm::Constant * CompareExprLT::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLT, left, right);
}

//-----------------------------------------------------------------------------
// CompareExprLE
//-----------------------------------------------------------------------------

CompareExprLE::CompareExprLE(ShiftExpr & leftExpr, CompareExpr & rightExpr) : CompareExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CompareExprLE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLE(left, right, "CompareExprLE_CmpOp");
}

llvm::Constant * CompareExprLE::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SLE, left, right);
}

//-----------------------------------------------------------------------------
// CompareExprGT
//-----------------------------------------------------------------------------

CompareExprGT::CompareExprGT(ShiftExpr & leftExpr, CompareExpr & rightExpr) : CompareExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CompareExprGT::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGT(left, right, "CompareExprGT_CmpOp");
}

llvm::Constant * CompareExprGT::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGT, left, right);
}

//-----------------------------------------------------------------------------
// CompareExprGE
//-----------------------------------------------------------------------------

CompareExprGE::CompareExprGE(ShiftExpr & leftExpr, CompareExpr & rightExpr) : CompareExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * CompareExprGE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGE(left, right, "CompareExprGE_CmpOp");
}

llvm::Constant * CompareExprGE::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Constant * left = mLeftExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Constant * right = mRightExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(right, nullptr);
    return llvm::ConstantExpr::getICmp(llvm::ICmpInst::Predicate::ICMP_SGE, left, right);
}

WC_END_NAMESPACE
