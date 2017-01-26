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

IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(Inc)
IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC(Dec)

#undef IMPL_DEFAULT_CODEGEN_UNARY_OP_FUNC
#endif

WC_END_NAMESPACE
