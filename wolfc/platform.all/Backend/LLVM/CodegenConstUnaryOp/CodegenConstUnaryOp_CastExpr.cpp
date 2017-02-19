#include "CodegenConstUnaryOp_CastExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "AST/Nodes/Type.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Base class for a unary op that converts from one type to many other types
//-----------------------------------------------------------------------------
class CodegenConstCastFromTypeUnaryOp : public DataTypeVisitor {
public:
    CodegenConstCastFromTypeUnaryOp(ConstCodegen & cg,
                                    const Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        mCG(cg),
        mFromConst(fromConst),
        mToTypeCDT(toTypeCDT)
    {
        WC_ASSERT(mFromConst.isValid());
        WC_ASSERT(mToTypeCDT.isValid());
    }
    
    /* This saves a lot of repetition */
    #define IMPL_UNSUPPORTED_CAST_OP_FUNC(DataTypeName)\
        virtual void visit(const DataTypeName##DataType & dataType) override {\
            WC_UNUSED_PARAM(dataType);\
            issueUnsupportedCastError();\
        }
    
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Array)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Bool)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Func)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int128)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int16)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int32)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int64)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int8)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Invalid)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Str)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt128)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt16)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt32)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt64)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt8)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Void)
    
    /* Done with this now! */
    #undef IMPL_UNSUPPORTED_CAST_OP_FUNC
    
    /* Save the operator result in the codegen context */
    void pushOpResult(llvm::Constant * result) {
        const AST::ASTNode * declaringNode = nullptr;
        const auto & astNodeStack = mCG.mCtx.getASTNodeStack();
        
        if (astNodeStack.empty()) {
            declaringNode = astNodeStack.back();
        }
        
        mCG.mCtx.pushConstant(Constant(result, mToTypeCDT, declaringNode));
    }
    
    /* The codegen function */
    void codegen() {
        mToTypeCDT.getDataType().accept(*this);
    }
    
protected:
    ConstCodegen &              mCG;
    const Constant &            mFromConst;
    const CompiledDataType &    mToTypeCDT;
    
private:
    void issueUnsupportedCastError() const {
        mCG.mCtx.error("Illegal cast! Cannot do a compile time cast from an expression of type '%s' to type '%s'!",
                       mFromConst.mCompiledType.getDataType().name().c_str(),
                       mToTypeCDT.getDataType().name().c_str());
    }
};

/* This macro saves a lot of repetition */
#define WC_IMPL_BASIC_CAST(CastGetFunc, ToDataTypeName)\
    virtual void visit(const ToDataTypeName##DataType & toType) override {\
        WC_UNUSED_PARAM(toType);\
        pushOpResult(llvm::ConstantExpr::CastGetFunc(mFromConst.mLLVMConst, mToTypeCDT.getLLVMType()));\
    }

//-----------------------------------------------------------------------------
// Cast From: Array
//-----------------------------------------------------------------------------
class CodegenConstCastFromArrayUnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromArrayUnaryOp(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: bool
//-----------------------------------------------------------------------------
class CodegenConstCastFromBoolUnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromBoolUnaryOp(ConstCodegen & cg,
                                    const Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: Func
//-----------------------------------------------------------------------------
class CodegenConstCastFromFuncUnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromFuncUnaryOp(ConstCodegen & cg,
                                    const Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: int128
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt128UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromInt128UnaryOp(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getBitCast, Int128)
    WC_IMPL_BASIC_CAST(getTrunc, Int64)
    WC_IMPL_BASIC_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getBitCast, UInt128)
    WC_IMPL_BASIC_CAST(getTrunc, UInt64)
    WC_IMPL_BASIC_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int16
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt16UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromInt16UnaryOp(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CAST(getSExt, Int64)
    WC_IMPL_BASIC_CAST(getSExt, Int32)
    WC_IMPL_BASIC_CAST(getBitCast, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CAST(getSExt, UInt64)
    WC_IMPL_BASIC_CAST(getSExt, UInt32)
    WC_IMPL_BASIC_CAST(getBitCast, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int32
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt32UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromInt32UnaryOp(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CAST(getSExt, Int64)
    WC_IMPL_BASIC_CAST(getBitCast, Int32)
    WC_IMPL_BASIC_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CAST(getSExt, UInt64)
    WC_IMPL_BASIC_CAST(getBitCast, UInt32)
    WC_IMPL_BASIC_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int64
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt64UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromInt64UnaryOp(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CAST(getBitCast, Int64)
    WC_IMPL_BASIC_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CAST(getBitCast, UInt64)
    WC_IMPL_BASIC_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int8
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt8UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromInt8UnaryOp(ConstCodegen & cg,
                                    const Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CAST(getSExt, Int64)
    WC_IMPL_BASIC_CAST(getSExt, Int32)
    WC_IMPL_BASIC_CAST(getSExt, Int16)
    WC_IMPL_BASIC_CAST(getBitCast, Int8)
    WC_IMPL_BASIC_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CAST(getSExt, UInt64)
    WC_IMPL_BASIC_CAST(getSExt, UInt32)
    WC_IMPL_BASIC_CAST(getSExt, UInt16)
    WC_IMPL_BASIC_CAST(getBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: str
//-----------------------------------------------------------------------------
class CodegenConstCastFromStrUnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromStrUnaryOp(ConstCodegen & cg,
                                   const Constant & fromConst,
                                   const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: uint128
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt128UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromUInt128UnaryOp(ConstCodegen & cg,
                                       const Constant & fromConst,
                                       const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getBitCast, Int128)
    WC_IMPL_BASIC_CAST(getTrunc, Int64)
    WC_IMPL_BASIC_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getBitCast, UInt128)
    WC_IMPL_BASIC_CAST(getTrunc, UInt64)
    WC_IMPL_BASIC_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint16
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt16UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromUInt16UnaryOp(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CAST(getZExt, Int64)
    WC_IMPL_BASIC_CAST(getZExt, Int32)
    WC_IMPL_BASIC_CAST(getBitCast, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CAST(getZExt, UInt64)
    WC_IMPL_BASIC_CAST(getZExt, UInt32)
    WC_IMPL_BASIC_CAST(getBitCast, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint32
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt32UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromUInt32UnaryOp(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CAST(getZExt, Int64)
    WC_IMPL_BASIC_CAST(getBitCast, Int32)
    WC_IMPL_BASIC_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CAST(getZExt, UInt64)
    WC_IMPL_BASIC_CAST(getBitCast, UInt32)
    WC_IMPL_BASIC_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint64
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt64UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromUInt64UnaryOp(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CAST(getBitCast, Int64)
    WC_IMPL_BASIC_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CAST(getBitCast, UInt64)
    WC_IMPL_BASIC_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint8
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt8UnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromUInt8UnaryOp(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CAST(getZExt, Int64)
    WC_IMPL_BASIC_CAST(getZExt, Int32)
    WC_IMPL_BASIC_CAST(getZExt, Int16)
    WC_IMPL_BASIC_CAST(getBitCast, Int8)
    WC_IMPL_BASIC_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CAST(getZExt, UInt64)
    WC_IMPL_BASIC_CAST(getZExt, UInt32)
    WC_IMPL_BASIC_CAST(getZExt, UInt16)
    WC_IMPL_BASIC_CAST(getBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: void
//-----------------------------------------------------------------------------
class CodegenConstCastFromVoidUnaryOp final : public CodegenConstCastFromTypeUnaryOp {
public:
    CodegenConstCastFromVoidUnaryOp(ConstCodegen & cg, const
                                    Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromTypeUnaryOp(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// CodegenConstCastUnaryOp
//-----------------------------------------------------------------------------
CodegenConstCastUnaryOp::CodegenConstCastUnaryOp(ConstCodegen & cg,
                                                 const AST::ASTNode & expr,
                                                 const AST::Type & toType)
:
    CodegenConstUnaryOp(
        cg,
        expr,
        "cast(<expr> to <type>)",
        "cast"
    ),
    mToType(toType)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstCastUnaryOp::codegen() {
    // Codegen the data type we are casting to
    mToType.accept(mCG);
    mToTypeCDT = mCG.mCtx.popCompiledDataType();
    
    // Call base class function to codegen the source expression and kick off visiting
    // the data type visitor functions for specific type casting:
    CodegenConstUnaryOp::codegen();
}

/* Macro to save a lot of repetition */
#define IMPL_CAST_FROM_TYPE_FUNC(FromTypeName)\
    void CodegenConstCastUnaryOp::visit(const FromTypeName##DataType & dataType) {\
        /* We don't use the input to these functions */\
        WC_UNUSED_PARAM(dataType);\
        /* If we don't have a valid type to cast to then bail */\
        WC_GUARD(mToTypeCDT.isValid());\
        \
        /**
         * If the data types of the source and target expression match then do nothing - no cast required.
         * In this case just push the constant back onto the codegen context to finish up.
         */\
        if (mToTypeCDT.getDataType().equals(mExprConst.mCompiledType.getDataType())) {\
            mCG.mCtx.pushConstant(mExprConst);\
            return;\
        }\
        \
        /* Okay, create the helper object which will do the cast and invoke it */\
        CodegenConstCastFrom##FromTypeName##UnaryOp(mCG,\
                                                    mExprConst,\
                                                    mToTypeCDT).codegen();\
    }

IMPL_CAST_FROM_TYPE_FUNC(Array)
IMPL_CAST_FROM_TYPE_FUNC(Bool)
IMPL_CAST_FROM_TYPE_FUNC(Func)
IMPL_CAST_FROM_TYPE_FUNC(Int128)
IMPL_CAST_FROM_TYPE_FUNC(Int16)
IMPL_CAST_FROM_TYPE_FUNC(Int32)
IMPL_CAST_FROM_TYPE_FUNC(Int64)
IMPL_CAST_FROM_TYPE_FUNC(Int8)
IMPL_CAST_FROM_TYPE_FUNC(Str)
IMPL_CAST_FROM_TYPE_FUNC(UInt128)
IMPL_CAST_FROM_TYPE_FUNC(UInt16)
IMPL_CAST_FROM_TYPE_FUNC(UInt32)
IMPL_CAST_FROM_TYPE_FUNC(UInt64)
IMPL_CAST_FROM_TYPE_FUNC(UInt8)
IMPL_CAST_FROM_TYPE_FUNC(Void)

/* Done with this */
#undef IMPL_CAST_FROM_FUNC

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
