#include "RandExpr.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "AssignExpr.hpp"
#include "DataType/DataType.hpp"
#include "DataType/DataTypeId.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"
#include "Module.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// RandExpr
//-----------------------------------------------------------------------------
bool RandExpr::peek(const Token * tokenPtr) {
    return  tokenPtr->type == TokenType::kRand ||
            tokenPtr->type == TokenType::kSRand;
}

RandExpr * RandExpr::parse(ParseCtx & parseCtx) {
    TokenType tokType = parseCtx.tok()->type;
    
    if (tokType == TokenType::kRand) {
        // rand() call: consume 'rand' and save start token
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Expect '('
        if (parseCtx.tok()->type != TokenType::kLParen) {
            parseCtx.error("Expect '(' following 'rand'!");
            return nullptr;
        }
        
        parseCtx.nextTok();     // Consume '('
        
        // Expect ')'
        if (parseCtx.tok()->type != TokenType::kRParen) {
            parseCtx.error("Expect ')' following 'rand('!");
            return nullptr;
        }
        
        // Consume ')', save end token and then return parsed node
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        return WC_NEW_AST_NODE(parseCtx, RandExprRand, *startToken, *endToken);
    }
    else if (tokType == TokenType::kSRand) {
        // srand() call: consume 'srand' and save start token
        const Token * startToken = parseCtx.tok();
        parseCtx.nextTok();
        
        // Expect '('
        if (parseCtx.tok()->type != TokenType::kLParen) {
            parseCtx.error("Expect '(' following 'srand'!");
            return nullptr;
        }
        
        parseCtx.nextTok();     // Consume '('
        
        // Parse the inner assign expression for the seed
        AssignExpr * seedExpr = AssignExpr::parse(parseCtx);
        WC_GUARD(seedExpr, nullptr);
        
        // Expect ')'
        if (parseCtx.tok()->type != TokenType::kRParen) {
            parseCtx.error("Expect ')' to close 'srand()' call!");
            return nullptr;
        }
        
        // Consume ')', save end token and then return parsed node
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();
        return WC_NEW_AST_NODE(parseCtx, RandExprSRand, *startToken, *seedExpr, *endToken);
    }
    
    parseCtx.error("Expected 'rand' or 'srand' token!");
    return nullptr;
}

RandExpr::RandExpr(const Token & startToken, const Token & endToken) :
    mStartToken(startToken),
    mEndToken(endToken)
{
    WC_EMPTY_FUNC_BODY();
}

const Token & RandExpr::getStartToken() const {
    return mStartToken;
}

const Token & RandExpr::getEndToken() const {
    return mEndToken;
}

bool RandExpr::isLValue() const {
    return false;
}

bool RandExpr::isConstExpr() const {
    return false;
}

#warning FIXME - Codegen
#if 0
llvm::Value * RandExpr::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of a 'rand()' or 'srand()' expression result!");
    return nullptr;
}

llvm::Constant * RandExpr::codegenExprConstEval(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Cannot evaluate a 'rand()' or 'srand()' expression at compile time!");
    return nullptr;
}
#endif

//-----------------------------------------------------------------------------
// RandExprRand
//-----------------------------------------------------------------------------
RandExprRand::RandExprRand(const Token & startToken, const Token & endToken) :
    RandExpr(startToken, endToken)
{
    WC_EMPTY_FUNC_BODY();
}

void RandExprRand::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const DataType & RandExprRand::dataType() const {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64);
}

#warning FIXME - Codegen
#if 0
llvm::Value * RandExprRand::codegenExprEval(CodegenCtx & cgCtx) {
    // Get 'rand' C function
    llvm::Constant * randFn = cgCtx.module.getLLVMModuleRef().getFunction("rand");
    
    if (!randFn) {
        compileError("Codegen failed! Can't find 'rand' function!");
        return nullptr;
    }
    
    // Create the call to rand!
    llvm::Value * randResult = cgCtx.irBuilder.CreateCall(randFn, {}, "RandExprRand:result");
    WC_ASSERT(randResult);
    
    // It needs to be converted to an int64:
    llvm::Type * int64Type = cgCtx.irBuilder.getInt64Ty();
    WC_ASSERT(int64Type);
    llvm::Value * extendedResult = cgCtx.irBuilder.CreateSExt(randResult, int64Type);
    WC_ASSERT(extendedResult);
    return extendedResult;
}
#endif

//-----------------------------------------------------------------------------
// RandExprSRand
//-----------------------------------------------------------------------------
RandExprSRand::RandExprSRand(const Token & startToken,
                             AssignExpr & seedExpr,
                             const Token & endToken)
:
    RandExpr(startToken, endToken),
    mSeedExpr(seedExpr)
{
    mSeedExpr.mParent = this;
}

void RandExprSRand::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const DataType & RandExprSRand::dataType() const {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kVoid);
}

#warning FIXME - Codegen
#if 0
llvm::Value * RandExprSRand::codegenExprEval(CodegenCtx & cgCtx) {
    // Evaluate the seed expression:
    llvm::Value * seedExprVal = mSeedExpr.codegenExprEval(cgCtx);
    WC_GUARD(seedExprVal, nullptr);
    
    // The seed expr type must be 'int'
    DataType & seedDataType = mSeedExpr.dataType();
    
    if (!seedDataType.equals(PrimitiveDataTypes::getUsingTypeId(DataTypeId::kInt64))) {
        compileError("Data type for seed given to 'srand()' call must be 'int' not '%s'!",
                     seedDataType.name().c_str());
        
        return nullptr;
    }
    
    // Get 'srand' C function
    llvm::Constant * srandFn = cgCtx.module.getLLVMModuleRef().getFunction("srand");
    
    if (!srandFn) {
        compileError("Codegen failed! Can't find 'srand' function!");
        return nullptr;
    }
    
    // Need to downcast the seed to a 32-bit int
    llvm::Type * int32Ty = cgCtx.irBuilder.getInt32Ty();
    WC_ASSERT(int32Ty);
    llvm::Value * seedExprVal32 = cgCtx.irBuilder.CreateTrunc(seedExprVal, int32Ty);
    WC_ASSERT(seedExprVal32);
    
    // Create the call to srand!
    llvm::Value * srandResult = cgCtx.irBuilder.CreateCall(srandFn, { seedExprVal32 });
    WC_ASSERT(srandResult);
    return srandResult;
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE