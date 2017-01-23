#include "WCDataType.hpp"

#include "AST/Nodes/WCASTNode.hpp"
#include "WCAssert.hpp"

WC_BEGIN_NAMESPACE

DataType::~DataType() {
    WC_EMPTY_FUNC_BODY();
}

bool DataType::isValid() const {
    return true;    // True for all types except 'Unknown'
}

bool DataType::isSized() const {
    return true;    // True for all types except 'Unknown' and 'Void'
}

bool DataType::isUnknown() const {
    return false;   // False for most datatypes except 'unknown' types
}

bool DataType::isInteger() const {
    return false;   // False by default for everything except integer types
}

bool DataType::isArray() const {
    return false;   // False by most types by default
}

bool DataType::requiresStorage() const {
    return false;   // False by default for all types
}

#warning FIXME - Codegen
#if 0
llvm::AllocaInst * DataType::codegenAlloca(CodegenCtx & cgCtx,
                                           AST::ASTNode & callingNode,
                                           const std::string & instLabel)
{
    WC_GUARD_ASSERT(mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
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
