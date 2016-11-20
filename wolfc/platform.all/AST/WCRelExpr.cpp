#include "WCRelExpr.hpp"

#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "Lexer/WCToken.hpp"
#include "WCAddSubExpr.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"
#include "WCLinearAlloc.hpp"

WC_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// RelExpr
//-----------------------------------------------------------------------------

bool RelExpr::peek(const Token * tokenPtr) {
    return AddSubExpr::peek(tokenPtr);
}

RelExpr * RelExpr::parse(const Token *& tokenPtr, LinearAlloc & alloc) {
    AddSubExpr * addSubExpr = AddSubExpr::parse(tokenPtr, alloc);
    WC_GUARD(addSubExpr, nullptr);
    
    // See what tokens follow:
    if (tokenPtr->type == TokenType::kLessThan) {
        ++tokenPtr;     // Skip '<'
        
        if (tokenPtr->type == TokenType::kEquals) {
            ++tokenPtr;     // Skip '='
            
            RelExpr * relExpr = RelExpr::parse(tokenPtr, alloc);
            WC_GUARD(relExpr, nullptr);
            return WC_NEW_AST_NODE(alloc, RelExprLE, *addSubExpr, *relExpr);
        }
        else {
            RelExpr * relExpr = RelExpr::parse(tokenPtr, alloc);
            WC_GUARD(relExpr, nullptr);
            return WC_NEW_AST_NODE(alloc, RelExprLT, *addSubExpr, *relExpr);
        }
    }
    else if (tokenPtr->type == TokenType::kGreaterThan) {
        ++tokenPtr;     // Skip '>'
        
        if (tokenPtr->type == TokenType::kEquals) {
            ++tokenPtr;     // Skip '='
            
            RelExpr * relExpr = RelExpr::parse(tokenPtr, alloc);
            WC_GUARD(relExpr, nullptr);
            return WC_NEW_AST_NODE(alloc, RelExprGE, *addSubExpr, *relExpr);
        }
        else {
            RelExpr * relExpr = RelExpr::parse(tokenPtr, alloc);
            WC_GUARD(relExpr, nullptr);
            return WC_NEW_AST_NODE(alloc, RelExprGT, *addSubExpr, *relExpr);
        }
    }

    // Basic no-op expression:
    return WC_NEW_AST_NODE(alloc, RelExprNoOp, *addSubExpr);
}

//-----------------------------------------------------------------------------
// RelExprNoOp
//-----------------------------------------------------------------------------

RelExprNoOp::RelExprNoOp(AddSubExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

const Token & RelExprNoOp::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & RelExprNoOp::getEndToken() const {
    return mExpr.getEndToken();
}

bool RelExprNoOp::isLValue() {
    return mExpr.isLValue();
}

bool RelExprNoOp::isConstExpr() {
    return mExpr.isConstExpr();
}

DataType & RelExprNoOp::dataType() {
    return mExpr.dataType();
}

llvm::Value * RelExprNoOp::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * RelExprNoOp::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * RelExprNoOp::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}

//-----------------------------------------------------------------------------
// RelExprTwoOps
//-----------------------------------------------------------------------------

RelExprTwoOps::RelExprTwoOps(AddSubExpr & leftExpr, RelExpr & rightExpr) :
    mLeftExpr(leftExpr),
    mRightExpr(rightExpr)
{
    mLeftExpr.mParent = this;
    mRightExpr.mParent = this;
}

const Token & RelExprTwoOps::getStartToken() const {
    return mLeftExpr.getStartToken();
}

const Token & RelExprTwoOps::getEndToken() const {
    return mRightExpr.getEndToken();
}

bool RelExprTwoOps::isLValue() {
    return false;
}

bool RelExprTwoOps::isConstExpr() {
    return mLeftExpr.isConstExpr() && mRightExpr.isConstExpr();
}

DataType & RelExprTwoOps::dataType() {
    return PrimitiveDataTypes::get(PrimitiveDataTypes::Type::kBool);
}

llvm::Value * RelExprTwoOps::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of relational operation result!");
    return nullptr;
}

bool RelExprTwoOps::compileCheckBothExprsAreInt() const {
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
// RelExprLT
//-----------------------------------------------------------------------------

RelExprLT::RelExprLT(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprLT::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLT(left, right, "RelExprLT_CmpOp");
}

llvm::Constant * RelExprLT::codegenExprConstEval(CodegenCtx & cgCtx) {
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
// RelExprLE
//-----------------------------------------------------------------------------

RelExprLE::RelExprLE(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprLE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSLE(left, right, "RelExprLE_CmpOp");
}

llvm::Constant * RelExprLE::codegenExprConstEval(CodegenCtx & cgCtx) {
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
// RelExprGT
//-----------------------------------------------------------------------------

RelExprGT::RelExprGT(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprGT::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGT(left, right, "RelExprGT_CmpOp");
}

llvm::Constant * RelExprGT::codegenExprConstEval(CodegenCtx & cgCtx) {
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
// RelExprGE
//-----------------------------------------------------------------------------

RelExprGE::RelExprGE(AddSubExpr & leftExpr, RelExpr & rightExpr) : RelExprTwoOps(leftExpr, rightExpr) {
    WC_EMPTY_FUNC_BODY();
}

llvm::Value * RelExprGE::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: handle auto type promotion and other non int types
    WC_GUARD(compileCheckBothExprsAreInt(), nullptr);
    
    // Generate code for the operation
    llvm::Value * left = mLeftExpr.codegenExprEval(cgCtx);
    WC_GUARD(left, nullptr);
    llvm::Value * right = mRightExpr.codegenExprEval(cgCtx);
    WC_GUARD(right, nullptr);
    return cgCtx.irBuilder.CreateICmpSGE(left, right, "RelExprGE_CmpOp");
}

llvm::Constant * RelExprGE::codegenExprConstEval(CodegenCtx & cgCtx) {
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
