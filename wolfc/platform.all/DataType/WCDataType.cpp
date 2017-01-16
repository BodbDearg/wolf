#include "WCDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "WCAssert.hpp"

#warning FIXME - Codegen
#if 0
#include "WCCodegenCtx.hpp"
#endif

#warning FIXME - Codegen
#if 0
WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END
#endif

WC_BEGIN_NAMESPACE

const char * const DataType::kOpSymbol_BNot = "~";
const char * const DataType::kOpSymbol_CmpEQ = "==";
const char * const DataType::kOpSymbol_CmpNE = "!=";
const char * const DataType::kOpSymbol_CmpLT = "<";
const char * const DataType::kOpSymbol_CmpLE = "<=";
const char * const DataType::kOpSymbol_CmpGT = ">";
const char * const DataType::kOpSymbol_CmpGE = ">=";
const char * const DataType::kOpSymbol_Add = "+";
const char * const DataType::kOpSymbol_Sub = "-";
const char * const DataType::kOpSymbol_BOr = "|";
const char * const DataType::kOpSymbol_BXOr = "^";
const char * const DataType::kOpSymbol_Mul = "*";
const char * const DataType::kOpSymbol_Div = "/";
const char * const DataType::kOpSymbol_Mod = "%";
const char * const DataType::kOpSymbol_BAnd = "&";
const char * const DataType::kOpSymbol_LShift = "<<";
const char * const DataType::kOpSymbol_ARShift = ">>";
const char * const DataType::kOpSymbol_LRShift = ">>>";
const char * const DataType::kOpSymbol_Plus = "+";
const char * const DataType::kOpSymbol_Minus = "-";
const char * const DataType::kOpSymbol_Inc = "++";
const char * const DataType::kOpSymbol_Dec = "--";

const char * const DataType::kOpName_BNot = "bitwise not";
const char * const DataType::kOpName_CmpEQ = "equals";
const char * const DataType::kOpName_CmpNE = "not equal to";
const char * const DataType::kOpName_CmpLT = "less than";
const char * const DataType::kOpName_CmpLE = "less than or equal to";
const char * const DataType::kOpName_CmpGT = "greater than";
const char * const DataType::kOpName_CmpGE = "greater than or equal to";
const char * const DataType::kOpName_Add = "add";
const char * const DataType::kOpName_Sub = "subtract";
const char * const DataType::kOpName_BOr = "bitwise or";
const char * const DataType::kOpName_BXOr = "bitwise xor";
const char * const DataType::kOpName_Mul = "multiply";
const char * const DataType::kOpName_Div = "divide";
const char * const DataType::kOpName_Mod = "modulus";
const char * const DataType::kOpName_BAnd = "bitwise and";
const char * const DataType::kOpName_LShift = "left shift";
const char * const DataType::kOpName_ARShift = "arithmetic right shift";
const char * const DataType::kOpName_LRShift = "logical right shift";
const char * const DataType::kOpName_Plus = "plus";
const char * const DataType::kOpName_Minus = "minus";
const char * const DataType::kOpName_Inc = "increment";
const char * const DataType::kOpName_Dec = "decrement";

DataType::~DataType() {
    WC_EMPTY_FUNC_BODY();
}

bool DataType::isValid() const {
    return true;    // True for all types except 'Unknown'
}

bool DataType::isSized() const {
    return true;    // True for all types except 'Unknown' and 'Void'
}

bool DataType::isInteger() const {
    return false;   // False by default for everything except integer types
}

bool DataType::requiresStorage() const {
    return false;   // False by default for all types
}

#warning FIXME - Codegen
#if 0
bool DataType::codegenLLVMTypeIfRequired(CodegenCtx & cgCtx, AST::ASTNode & callingNode) {
    if (mLLVMType) {
        return true;
    }
    
    return codegenLLVMType(cgCtx, callingNode);
}

llvm::AllocaInst * DataType::codegenAlloca(CodegenCtx & cgCtx,
                                           AST::ASTNode & callingNode,
                                           const std::string & instLabel)
{
    WC_GUARD_ASSERT(mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
}

bool DataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                 AST::ASTNode & callingNode,
                                 llvm::Constant & printfFn,
                                 llvm::Value & valToPrint)
{
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(valToPrint);
    callingNode.compileError("print() is not supported for expression of type '%s'!", name().c_str());
    return false;
}

llvm::Value * DataType::codegenCastTo(CodegenCtx & cgCtx,
                                      AST::ASTNode & callingNode,
                                      llvm::Value & valToCast,
                                      DataType & castToTy)
{
    // If the type to cast to is the same as this type then do nothing, cast not needed:
    if (equals(castToTy)) {
        return &valToCast;
    }
    
    // The default impl of this just raises an error
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(valToCast);
    
    callingNode.compileError("Cast from type left type '%s' to right type '%s' is not allowed!",
                             name().c_str(),
                             castToTy.name().c_str());
    
    return nullptr;
}
#endif

#warning FIXME - Codegen
#if 0
/* Default binary op implementations */
#define IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(OpName)\
    llvm::Value * DataType::codegen ## OpName ## Op(CodegenCtx & cgCtx,\
                                                    AST::ASTNode & callingNode,\
                                                    llvm::Value & leftVal,\
                                                    DataType & rightTy,\
                                                    llvm::Value & rightVal)\
    {\
        /* The default impl simply issues an error that the operator is not available */\
        WC_UNUSED_PARAM(cgCtx);\
        WC_UNUSED_PARAM(leftVal);\
        WC_UNUSED_PARAM(rightVal);\
        issueBinaryOpNotAvailableCompileError(callingNode, kOpSymbol_ ## OpName, kOpName_ ## OpName, rightTy);\
        return nullptr;\
    }

IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(CmpEQ)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(CmpNE)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(CmpLT)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(CmpLE)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(CmpGT)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(CmpGE)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(Add)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(Sub)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(BOr)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(BXOr)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(Mul)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(Div)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(Mod)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(BAnd)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(LShift)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(ARShift)
IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC(LRShift)

#undef IMPL_DEFAULT_CODEGEN_BINARY_OP_FUNC

/* Default unary op implementations */
#define IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(OpName)\
    llvm::Value * DataType::codegen ## OpName ## Op(CodegenCtx & cgCtx,\
                                                    AST::ASTNode & callingNode,\
                                                    llvm::Value & val)\
    {\
        /* The default impl simply issues an error that the operator is not available */\
        WC_UNUSED_PARAM(cgCtx);\
        WC_UNUSED_PARAM(val);\
        issueUnaryOpNotAvailableCompileError(callingNode, kOpSymbol_ ## OpName, kOpName_ ## OpName);\
        return nullptr;\
    }

IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(BNot)
IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(Plus)
IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(Minus)
IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(Inc)
IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(Dec)

#undef IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC

/* Default const binary op implementations */
#define IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(OpName)\
    llvm::Constant * DataType::codegenConst ## OpName ## Op(AST::ASTNode & callingNode,\
                                                            llvm::Constant & leftVal,\
                                                            DataType & rightTy,\
                                                            llvm::Constant & rightVal)\
    {\
        /* The default impl simply issues an error that the operator is not available */\
        WC_UNUSED_PARAM(leftVal);\
        WC_UNUSED_PARAM(rightVal);\
        issueBinaryOpNotAvailableCompileError(callingNode, kOpSymbol_ ## OpName, kOpName_ ## OpName, rightTy);\
        return nullptr;\
    }

IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(CmpEQ)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(CmpNE)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(CmpLT)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(CmpLE)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(CmpGT)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(CmpGE)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(Add)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(Sub)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(BOr)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(BXOr)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(Mul)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(Div)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(Mod)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(BAnd)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(LShift)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(ARShift)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(LRShift)

#undef IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC

/* Default const unary op implementations */
#define IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(OpName)\
    llvm::Constant * DataType::codegenConst ## OpName ## Op(AST::ASTNode & callingNode,\
                                                            llvm::Constant & val)\
    {\
        /* The default impl simply issues an error that the operator is not available */\
        WC_UNUSED_PARAM(val);\
        issueUnaryOpNotAvailableCompileError(callingNode, kOpSymbol_ ## OpName, kOpName_ ## OpName);\
        return nullptr;\
    }

IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(BNot)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(Plus)
IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC(Minus)

#undef IMPL_DEFAULT_CODEGEN_CONST_BINARY_OP_FUNC
#endif

#warning FIXME - Codegen
#if 0
bool DataType::compileCheckLLVMTypeDefined(AST::ASTNode & callingNode) {
    WC_GUARD(!mLLVMType, true);
    callingNode.compileError("The LLVM type for data type '%s' is undefined!", name().c_str());
    return false;
}

bool DataType::compileCheckBinaryOpTypesMatch(AST::ASTNode & callingNode,
                                              const char * opSymbol,
                                              const char * opName,
                                              DataType & rightExprType)
{
    // If the types match then all is ok
    if (equals(rightExprType)) {
        return true;
    }
    
    // Issue the compile error:
    callingNode.compileError("The types on both sides of the '%s' (%s) operator are incompatible! "
                             "The left expression type '%s' does not match the right expression type '%s'!",
                             opSymbol,
                             opName,
                             name().c_str(),
                             rightExprType.name().c_str());
    
    // Check failed:
    return false;
}

void DataType::issueBinaryOpNotAvailableCompileError(AST::ASTNode & callingNode,
                                                     const char * opSymbol,
                                                     const char * opName,
                                                     DataType & rightExprType) const
{
    callingNode.compileError("The '%s' (%s) binary operator is not supported for a left "
                             "expression of type '%s' with a right expression of type '%s'!",
                             opSymbol,
                             opName,
                             name().c_str(),
                             rightExprType.name().c_str());
}

void DataType::issueUnaryOpNotAvailableCompileError(AST::ASTNode & callingNode,
                                                    const char * opSymbol,
                                                    const char * opName) const
{
    callingNode.compileError("The '%s' (%s) unary operator is not supported for an "
                             "expression of type '%s'!",
                             opSymbol,
                             opName,
                             name().c_str());
}

void DataType::issueGenericCodegenLLVMTypeError(AST::ASTNode & callingNode) const {
    callingNode.compileError("Failed to codegen the llvm type for data type '%s'!", name().c_str());
}
#endif

WC_END_NAMESPACE
