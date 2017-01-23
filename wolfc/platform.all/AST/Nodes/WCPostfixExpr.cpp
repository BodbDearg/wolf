#include "WCPostfixExpr.hpp"

#include "AST/WCASTNodeVisitor.hpp"
#include "DataType/Primitives/WCArrayDataType.hpp"
#include "DataType/WCDataType.hpp"
#include "DataType/WCPrimitiveDataTypes.hpp"
#include "WCAssert.hpp"
#include "WCAssignExpr.hpp"
#include "WCCastExpr.hpp"
#include "WCFunc.hpp"
#include "WCFuncArg.hpp"
#include "WCFuncCall.hpp"
#include "WCIdentifier.hpp"
#include "WCLinearAlloc.hpp"
#include "WCModule.hpp"
#include "WCParseCtx.hpp"
#include "WCPrimaryExpr.hpp"
#include "WCPrimitiveType.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// PostfixExpr
//-----------------------------------------------------------------------------
bool PostfixExpr::peek(const Token * currentToken) {
    return CastExpr::peek(currentToken);
}

PostfixExpr * PostfixExpr::parse(ParseCtx & parseCtx) {
    // Parse the initial expression
    CastExpr * expr = CastExpr::parse(parseCtx);
    WC_GUARD(expr, nullptr);

    // Save the outermost postfix expression here:
    PostfixExpr * outerPostfixExpr = nullptr;

    // See if '++' or '--' follow:
    if (parseCtx.tok()->type == TokenType::kIncrement) {
        // Consume the '++' token and save
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();

        // Create outer postfix expr
        outerPostfixExpr = WC_NEW_AST_NODE(parseCtx, PostfixExprInc, *expr, *endToken);
        WC_ASSERT(outerPostfixExpr);
    }
    else if (parseCtx.tok()->type == TokenType::kDecrement) {
        // Consume the '--' token and save
        const Token * endToken = parseCtx.tok();
        parseCtx.nextTok();

        // Create outer postfix expr
        outerPostfixExpr = WC_NEW_AST_NODE(parseCtx, PostfixExprDec, *expr, *endToken);
        WC_ASSERT(outerPostfixExpr);
    }
    else {
        // Basic postfix outer expression with no increment or decrement
        outerPostfixExpr = WC_NEW_AST_NODE(parseCtx, PostfixExprNoPostfix, *expr);
        WC_ASSERT(outerPostfixExpr);
    }
    
    // Continue parsing and wrapping until one of these conditions breaks
    while (FuncCall::peek(parseCtx.tok()) ||
           parseCtx.tok()->type == TokenType::kLBrack)
    {
        // See if function call follows:
        if (FuncCall::peek(parseCtx.tok())) {
            FuncCall * funcCall = FuncCall::parse(parseCtx);
            WC_GUARD(funcCall, nullptr);
            outerPostfixExpr = WC_NEW_AST_NODE(parseCtx,
                                               PostfixExprFuncCall,
                                               *outerPostfixExpr,
                                               *funcCall);
            
            WC_ASSERT(outerPostfixExpr);
        }
        else {
            // Skip the '['. Expect '[' to be here based on previous if() failing - see while loop.
            WC_ASSERT(parseCtx.tok()->type == TokenType::kLBrack);
            parseCtx.nextTok();
            
            // Parse the assign expression for the array index
            AssignExpr * arrayIndexExpr = AssignExpr::parse(parseCtx);
            WC_GUARD(arrayIndexExpr, nullptr);
            
            // Expect a closing ']'
            const Token & endToken = *parseCtx.tok();
            
            if (endToken.type != TokenType::kRBrack) {
                parseCtx.error("Expected a closing ']' for array lookup expression!");
                return nullptr;
            }
            
            // Consume the ']' and create the outer postfix expression
            parseCtx.nextTok();
            outerPostfixExpr = WC_NEW_AST_NODE(parseCtx,
                                               PostfixExprArrayLookup,
                                               *outerPostfixExpr,
                                               *arrayIndexExpr,
                                               endToken);
            
            WC_ASSERT(outerPostfixExpr);
        }
    }
    
    return outerPostfixExpr;
}

//-----------------------------------------------------------------------------
// PostfixExprNoPostfix
//-----------------------------------------------------------------------------
PostfixExprNoPostfix::PostfixExprNoPostfix(CastExpr & expr) : mExpr(expr) {
    mExpr.mParent = this;
}

void PostfixExprNoPostfix::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PostfixExprNoPostfix::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprNoPostfix::getEndToken() const {
    return mExpr.getEndToken();
}

bool PostfixExprNoPostfix::isLValue() const {
    return mExpr.isLValue();
}

bool PostfixExprNoPostfix::isConstExpr() const {
    return mExpr.isConstExpr();
}

const DataType & PostfixExprNoPostfix::dataType() const {
    return mExpr.dataType();
}

#warning FIXME - Codegen
#if 0
llvm::Value * PostfixExprNoPostfix::codegenAddrOf(CodegenCtx & cgCtx) {
    return mExpr.codegenAddrOf(cgCtx);
}

llvm::Value * PostfixExprNoPostfix::codegenExprEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprEval(cgCtx);
}

llvm::Constant * PostfixExprNoPostfix::codegenExprConstEval(CodegenCtx & cgCtx) {
    return mExpr.codegenExprConstEval(cgCtx);
}
#endif

//-----------------------------------------------------------------------------
// PostfixExprIncDecBase
//-----------------------------------------------------------------------------
PostfixExprIncDecBase::PostfixExprIncDecBase(CastExpr & expr,
                                             const Token & endToken
                                            #warning FIXME - Codegen
                                            #if 0
                                             ,DTCodegenUnaryOpFunc codegenUnaryOpFunc
                                            #endif
                                             )
:
    mExpr(expr),
    mEndToken(endToken)
#warning FIXME - Codegen
#if 0
    ,mCodegenUnaryOpFunc(codegenUnaryOpFunc)
#endif
{
#warning FIXME - Codegen
#if 0
    WC_ASSERT(mCodegenUnaryOpFunc);
#endif
    mExpr.mParent = this;
}

const Token & PostfixExprIncDecBase::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprIncDecBase::getEndToken() const {
    return mEndToken;
}

bool PostfixExprIncDecBase::isLValue() const {
    return false;
}

bool PostfixExprIncDecBase::isConstExpr() const {
    // Not allowed to use in constant expressions
    // TODO: can this be relaxed in future for functions that can be evaluated at compile time?
    return false;
}

const DataType & PostfixExprIncDecBase::dataType() const {
    // Unlike C/C++ increment and decrement expressions do not return a value. This removes the need for prefix/postfix 
    // increment and prevents them from being used in a confusing way in expressions.
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kVoid);
}

#warning FIXME - Codegen
#if 0
llvm::Value * PostfixExprIncDecBase::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Cant codegen the address of an increment or decrement statement!");
    return nullptr;
}

llvm::Value * PostfixExprIncDecBase::codegenExprEval(CodegenCtx & cgCtx) {
    // Get the address of the expression
    llvm::Value * exprAddr = mExpr.codegenAddrOf(cgCtx);
    WC_GUARD(exprAddr, nullptr);

    // Load it's current value
    llvm::Value * exprOldValue = cgCtx.irBuilder.CreateLoad(exprAddr, "PostfixExprIncDecBase:Load");
    WC_ASSERT(exprOldValue);

    // Do the increment:
    DataType & exprType = mExpr.dataType();
    llvm::Value * exprNewValue = (exprType.*mCodegenUnaryOpFunc)(cgCtx, *this, *exprOldValue);
    WC_GUARD(exprNewValue, nullptr);

    // Save out the result
    llvm::Value * storeInst = cgCtx.irBuilder.CreateStore(exprNewValue, exprAddr);
    WC_ASSERT(storeInst);
    return storeInst;
}

llvm::Constant * PostfixExprIncDecBase::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Not allowed to use in constant expressions
    // TODO: can this be relaxed in future for functions that can be evaluated at compile time?
    WC_UNUSED_PARAM(cgCtx);
    compileError("Increment and decrement operators cannot be used in constant expressions!");
    return nullptr;
}
#endif

//-----------------------------------------------------------------------------
// PostfixExprInc
//-----------------------------------------------------------------------------
PostfixExprInc::PostfixExprInc(CastExpr & expr, const Token & endToken) :
    PostfixExprIncDecBase(expr, endToken
                        #warning FIXME - Codegen
                        #if 0
                          , &DataType::codegenIncOp
                        #endif
                          )
{
    WC_EMPTY_FUNC_BODY();
}

void PostfixExprInc::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PostfixExprDec
//-----------------------------------------------------------------------------
PostfixExprDec::PostfixExprDec(CastExpr & expr, const Token & endToken) :
    PostfixExprIncDecBase(expr, endToken
                        #warning FIXME - Codegen
                        #if 0
                          , &DataType::codegenDecOp
                        #endif
                          )
{
    WC_EMPTY_FUNC_BODY();
}

void PostfixExprDec::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

//-----------------------------------------------------------------------------
// PostfixExprFuncCall
//-----------------------------------------------------------------------------
PostfixExprFuncCall::PostfixExprFuncCall(PostfixExpr & expr, FuncCall & funcCall) :
    mExpr(expr),
    mFuncCall(funcCall)
#warning FIXME - Codegen
#if 0
    ,mAddrOfResult(nullptr),
    mExprEvalResult(nullptr)
#endif
{
    mExpr.mParent = this;
    mFuncCall.mParent = this;
}

void PostfixExprFuncCall::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PostfixExprFuncCall::getStartToken() const {
    return mExpr.getStartToken();
}

const Token & PostfixExprFuncCall::getEndToken() const {
    return mFuncCall.getEndToken();
}

bool PostfixExprFuncCall::isLValue() const {
    // Can treat arrays returned from a function as an l-value, which can be dereferenced,
    // but not simple types that are stored in registers..
    return dataType().requiresStorage();
}

bool PostfixExprFuncCall::isConstExpr() const {
    // TODO: Someday support calling functions that can be evaluated at compile time
    return false;
}

const DataType & PostfixExprFuncCall::dataType() const {
#warning FIXME - Codegen
#if 0
    Func * funcCalled = lookupFuncCalled();
    
    if (funcCalled) {
        return funcCalled->returnDataType();
    }
#endif
    
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kUnknown);
}

#warning FIXME - Codegen
#if 0
llvm::Value * PostfixExprFuncCall::codegenAddrOf(CodegenCtx & cgCtx) {
    // If already done then just return the previous calculated result
    WC_GUARD(!mAddrOfResult, mAddrOfResult);
    
    // This is only possible if the data type requires storage
    DataType & returnDataType = dataType();
    
    if (!returnDataType.requiresStorage()) {
        compileError("Can't get the address of function call result!");
        return nullptr;
    }
    
    // Make sure the expression was evaluated
    codegenExprEval(cgCtx);
    WC_GUARD(mExprEvalResult, nullptr);
    
    // Codegen the return data type if required
    WC_GUARD(returnDataType.codegenLLVMTypeIfRequired(cgCtx, *this), nullptr);
    
    // Create an alloc to hold the result of the function call.
    // This will be what we return:
    mAddrOfResult = cgCtx.irBuilder.CreateAlloca(returnDataType.mLLVMType);
    WC_ASSERT(mAddrOfResult);
    
    // Store the function call result in
    cgCtx.irBuilder.CreateStore(mExprEvalResult, mAddrOfResult);
    
    // Return the result
    return mAddrOfResult;
}

llvm::Value * PostfixExprFuncCall::codegenExprEval(CodegenCtx & cgCtx) {
    // If already done then just return the previous calculated result
    WC_GUARD(!mExprEvalResult, mExprEvalResult);
    
    // Name of function to call:
    const char * funcName = nameOfFuncCalled();
    
    if (!funcName) {
        compileError("Function to call must be specified by a single identifier!");
        return nullptr;
    }
    
    // Get the function to call:
    Func * func = cgCtx.module.getFunc(funcName);
    
    if (!func) {
        compileError("No such function named '%s' exists to call!", funcName);
        return nullptr;
    }
    
    // Verify the number of arguments being given to the function is correct:
    std::vector<FuncArg*> funcArgs;
    func->getArgs(funcArgs);
    
    std::vector<AssignExpr*> callArgs;
    mFuncCall.getArgs(callArgs);
    size_t numFuncArgs = funcArgs.size();
    
    if (numFuncArgs != callArgs.size()) {
        compileError("Invalid number of argumenst for function call! Expected %zu instead of %zu!",
                     numFuncArgs,
                     callArgs.size());
        
        return nullptr;
    }
    
    // Generate the code for the arguments list of the function call:
    WC_GUARD(mFuncCall.codegenArgsListExprs(cgCtx), nullptr);

    // Verify the type of each argument is correct:
    for (size_t i = 0; i < numFuncArgs; ++i) {
        const DataType & funcArgDataType = funcArgs[i]->dataType();
        const DataType & callArgDataType = callArgs[i]->dataType();

        // TODO: support auto promotion
        if (!funcArgDataType.equals(callArgDataType)) {
            compileError("Type for arg number '%zu' in function call is invalid! Expected '%s' "
                "but instead got '%s'!",
                i + 1,
                funcArgDataType.name().c_str(),
                callArgDataType.name().c_str());

            return nullptr;
        }
    }
    
    // Call it: note if non void we have to give the value a name
    if (func->returnDataType().isVoid()) {
        mExprEvalResult = cgCtx.irBuilder.CreateCall(func->mLLVMFunc,
                                                     mFuncCall.mArgListExprsValues);
    }
    else {
        mExprEvalResult = cgCtx.irBuilder.CreateCall(func->mLLVMFunc,
                                                     mFuncCall.mArgListExprsValues,
                                                     "PostfixExprFuncCall:ReturnVal");
    }
    
    return mExprEvalResult;
}

llvm::Constant * PostfixExprFuncCall::codegenExprConstEval(CodegenCtx & cgCtx) {
    // TODO: some day, eventually, support compile time function evaluation
    WC_UNUSED_PARAM(cgCtx);
    compileError("Cannot call functions in a constant expression!");
    return nullptr;
}
#endif

const char * PostfixExprFuncCall::nameOfFuncCalled() const {
    // TODO: support member function calls on this object (some day)
    // TODO: support member function calls on another object (some day)
    // TODO: support lambda calls (some day)
    // TODO: support built in functions on basic types (somenum.isNan() etc.)
    
    PostfixExprNoPostfix * postfixExprNoPostfix = dynamic_cast<PostfixExprNoPostfix*>(&mExpr);
    WC_GUARD(postfixExprNoPostfix, nullptr);
    CastExprNoCast * castExprNoCast = dynamic_cast<CastExprNoCast*>(&postfixExprNoPostfix->mExpr);
    WC_GUARD(castExprNoCast, nullptr);
    PrimaryExprIdentifier * primaryExprIdentifier = dynamic_cast<PrimaryExprIdentifier*>(&castExprNoCast->mExpr);
    WC_GUARD(primaryExprIdentifier, nullptr);
    
    return primaryExprIdentifier->name();
}

#warning FIXME - Codegen
#if 0
Func * PostfixExprFuncCall::lookupFuncCalled() const {
    // Get the function name
    const char * funcName = nameOfFuncCalled();
    WC_GUARD(funcName, nullptr);
    
    // Lookup parent module - this should always exist
    const Module * parentModule = firstParentOfType<Module>();
    WC_ASSERT(parentModule);
    return parentModule->getFunc(funcName);
}
#endif

//-----------------------------------------------------------------------------
// PostfixExprArrayLookup
//-----------------------------------------------------------------------------
PostfixExprArrayLookup::PostfixExprArrayLookup(PostfixExpr & arrayExpr,
                                               AssignExpr & indexExpr,
                                               const Token & endToken)
:
    mArrayExpr(arrayExpr),
    mIndexExpr(indexExpr),
    mEndToken(endToken)
{
    mArrayExpr.mParent = this;
    mIndexExpr.mParent = this;
}

void PostfixExprArrayLookup::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & PostfixExprArrayLookup::getStartToken() const {
    return mArrayExpr.getStartToken();
}

const Token & PostfixExprArrayLookup::getEndToken() const {
    return mEndToken;
}

bool PostfixExprArrayLookup::isLValue() const {
    return mArrayExpr.isLValue();
}

bool PostfixExprArrayLookup::isConstExpr() const {
    return mArrayExpr.isConstExpr() && mIndexExpr.isConstExpr();
}

const DataType & PostfixExprArrayLookup::dataType() const {
    // Expect the array expression to have the array data type.
    // If it doesn't have this type then we don't know what the element type is..
    const ArrayDataType * arrayDataType = dynamic_cast<const ArrayDataType*>(&mArrayExpr.dataType());
    WC_GUARD(arrayDataType, PrimitiveDataTypes::getUsingTypeId(DataTypeId::kUnknown));
    
    // Return the element data type
    return arrayDataType->mInnerType;
}

#warning FIXME - Codegen
#if 0
llvm::Value * PostfixExprArrayLookup::codegenAddrOf(CodegenCtx & cgCtx) {
    return codegenAddrOfArrayElem(cgCtx);
}

llvm::Value * PostfixExprArrayLookup::codegenExprEval(CodegenCtx & cgCtx) {
    // Codegen the address of the array element
    llvm::Value * arrayElemAddr = codegenAddrOfArrayElem(cgCtx);
    WC_GUARD(arrayElemAddr, nullptr);

    // Get the array datatype
    ArrayDataType * arrayDataType = getArrayDataTypeOrIssueError();
    WC_GUARD(arrayDataType, nullptr);
    
    // If the element type is not an array then load, otherwise return it
    if (arrayDataType->mInnerType.isArray()) {
        return arrayElemAddr;
    }
    
    return cgCtx.irBuilder.CreateLoad(arrayElemAddr);
}

llvm::Constant * PostfixExprArrayLookup::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Codegen the array constant:
    llvm::ConstantArray * arrayConstant = static_cast<llvm::ConstantArray*>(mArrayExpr.codegenExprConstEval(cgCtx));
    WC_GUARD(arrayConstant, nullptr);
    
    // Codegen the index expression:
    llvm::Constant * indexConstant = mIndexExpr.codegenExprConstEval(cgCtx);
    WC_GUARD(indexConstant, nullptr);
    
    // Make sure the array is actually an array...
    // TODO: support this operator on custom types eventually
    if (!mArrayExpr.dataType().isArray()) {
        compileError("Can't perform array indexing on an expression that is not an array!");
        return nullptr;
    }
    
    // Index expression must be an integer
    if (!mIndexExpr.dataType().isInteger()) {
        mIndexExpr.compileError("Index expression for array lookup must be an integer not type '%s'! "
                                "Can't index an array with non-integer types!",
                                mIndexExpr.dataType().name().c_str());
        
        return nullptr;
    }
    
    // Make sure the index expression fits in 64-bits
    llvm::APInt indexAPInt = indexConstant->getUniqueInteger();
    
    if (indexAPInt.getActiveBits() > 64) {
        mIndexExpr.compileError("Index expression for array lookup is too big! Does not fit in 64-bits! "
                                "Number requires '%zu' bits to store!",
                                uint64_t(indexAPInt.getActiveBits()));
        
        return nullptr;
    }
    
    // Index expression cannot be negative
    if (indexAPInt.isNegative()) {
        mIndexExpr.compileError("Index expression for array lookup cannot be negative! "
                                "Value given was: %zi",
                                int64_t(indexAPInt.getSExtValue()));
        
        return nullptr;
    }
    
    uint64_t index = indexAPInt.getZExtValue();
    
    // Okay, make sure the index is in bounds
    if (index >= arrayConstant->getType()->getNumElements()) {
        mIndexExpr.compileError("Index '%zu' is out of range for the array! "
                                "Array size is: %zu",
                                index,
                                uint64_t(arrayConstant->getType()->getNumElements()));
        
        return nullptr;
    }
    
    // Due to LLVM API restrictions we can only support as big as UINT_MAX here for an index because
    // getExtractElement() takes this data type rather than uint64_t...
    //
    // It may be possible to lift this restriction in future, though I doubt this limitation will be
    // much of an issue in practice. Who throws arrays >~ 4 billion elements at their compilers anyways?!
    if (index > UINT_MAX) {
        compileError("Array index '%zu' is too large! "
                     "Can only support a max array index of '%zu' due to LLVM API restrictions!",
                     index,
                     uint64_t(UINT_MAX));
        
        return nullptr;
    }
    
    // Right, now get the constant
    return llvm::ConstantExpr::getExtractValue(arrayConstant, { static_cast<unsigned>(index) });
}

ArrayDataType * PostfixExprArrayLookup::getArrayDataTypeOrIssueError() {
    ArrayDataType * arrayDataType = dynamic_cast<ArrayDataType*>(&mArrayExpr.dataType());
    
    if (!arrayDataType) {
        compileError("Can't perform array indexing on an expression that is not an array!");
        return nullptr;
    }
    
    return arrayDataType;
}

llvm::Value * PostfixExprArrayLookup::codegenAddrOfArrayElem(CodegenCtx & cgCtx) {
    
    // TODO: support this operator on custom types eventually
    
    // Codgen the address of the array first
    llvm::Value * arrayAddr = mArrayExpr.codegenAddrOf(cgCtx);
    WC_GUARD(arrayAddr, nullptr);

    // Make sure the array is actually an array...
    // Note: we do this after codegen since that will catch more meaningful error info (like unknown var names)
    if (!mArrayExpr.dataType().isArray()) {
        compileError("Can't perform array indexing on an expression that is not an array!");
        return nullptr;
    }
    
    // Codegen the expression for the array index
    llvm::Value * indexValue = mIndexExpr.codegenExprEval(cgCtx);
    WC_GUARD(indexValue, nullptr);

    // Index expression must be an integer
    // Note: we do this after codegen since that will catch more meaningful error info (like unknown var names)
    if (!mIndexExpr.dataType().isInteger()) {
        mIndexExpr.compileError("Index expression for array lookup must be an integer not type '%s'! "
            "Can't index an array with non-integer types!",
            mIndexExpr.dataType().name().c_str());

        return nullptr;
    }
    
    // Get the value for the array address and return it:
    llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), 0);
    WC_ASSERT(zeroIndex);
    llvm::Value * arrayAddress = cgCtx.irBuilder.CreateGEP(arrayAddr, { zeroIndex, indexValue });
    WC_ASSERT(arrayAddress);
    return arrayAddress;
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
