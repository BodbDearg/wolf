#include "CodegenUnaryOp_CastExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/Type.hpp"
#include "Assert.hpp"
#include "DataType/DataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Base class for a unary op that converts from one type to many other types
//-----------------------------------------------------------------------------
class CodegenCastFromTypeUnaryOp : public DataTypeVisitor {
public:
    CodegenCastFromTypeUnaryOp(Codegen & cg,
                               const Value & fromVal,
                               const CompiledDataType & toTypeCDT)
    :
        mCG(cg),
        mFromVal(fromVal),
        mToTypeCDT(toTypeCDT)
    {
        WC_ASSERT(mFromVal.isValid());
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
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Ptr)
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
    void pushOpResult(llvm::Value * result) {
        const AST::ASTNode * declaringNode = nullptr;
        const auto & astNodeStack = mCG.mCtx.getASTNodeStack();
        
        if (astNodeStack.empty()) {
            declaringNode = astNodeStack.back();
        }
        
        mCG.mCtx.pushValue(Value(result, mToTypeCDT, false, declaringNode));
    }
    
    /* The codegen function */
    void codegen() {
        mToTypeCDT.getDataType().accept(*this);
    }
    
protected:
    Codegen &                   mCG;
    const Value &               mFromVal;
    const CompiledDataType &    mToTypeCDT;
    
private:
    void issueUnsupportedCastError() const {
        mCG.mCtx.error("Illegal cast! Cannot cast from an expression of type '%s' to type '%s'!",
                       mFromVal.mCompiledType.getDataType().name().c_str(),
                       mToTypeCDT.getDataType().name().c_str());
    }
};

/* This macro saves a lot of repetition */
#define WC_IMPL_BASIC_CAST(CastCreateFunc, ToDataTypeName)\
    virtual void visit(const ToDataTypeName##DataType & toType) override {\
        WC_UNUSED_PARAM(toType);\
        pushOpResult(mCG.mCtx.mIRBuilder.CastCreateFunc(mFromVal.mLLVMVal, mToTypeCDT.getLLVMType()));\
    }

//-----------------------------------------------------------------------------
// Cast From: Array
//-----------------------------------------------------------------------------
class CodegenCastFromArrayUnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromArrayUnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: bool
//-----------------------------------------------------------------------------
class CodegenCastFromBoolUnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromBoolUnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: Func
//-----------------------------------------------------------------------------
class CodegenCastFromFuncUnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromFuncUnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: int128
//-----------------------------------------------------------------------------
class CodegenCastFromInt128UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromInt128UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateBitCast, Int128)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt128)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int16
//-----------------------------------------------------------------------------
class CodegenCastFromInt16UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromInt16UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateSExt, Int64)
    WC_IMPL_BASIC_CAST(CreateSExt, Int32)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int32
//-----------------------------------------------------------------------------
class CodegenCastFromInt32UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromInt32UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateSExt, Int64)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int64
//-----------------------------------------------------------------------------
class CodegenCastFromInt64UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromInt64UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int8
//-----------------------------------------------------------------------------
class CodegenCastFromInt8UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromInt8UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateSExt, Int128)
    WC_IMPL_BASIC_CAST(CreateSExt, Int64)
    WC_IMPL_BASIC_CAST(CreateSExt, Int32)
    WC_IMPL_BASIC_CAST(CreateSExt, Int16)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int8)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateSExt, UInt16)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: str
//-----------------------------------------------------------------------------
class CodegenCastFromStrUnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromStrUnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: uint128
//-----------------------------------------------------------------------------
class CodegenCastFromUInt128UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromUInt128UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateBitCast, Int128)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt128)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint16
//-----------------------------------------------------------------------------
class CodegenCastFromUInt16UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromUInt16UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateZExt, Int64)
    WC_IMPL_BASIC_CAST(CreateZExt, Int32)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint32
//-----------------------------------------------------------------------------
class CodegenCastFromUInt32UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromUInt32UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateZExt, Int64)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint64
//-----------------------------------------------------------------------------
class CodegenCastFromUInt64UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromUInt64UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int64)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int32)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int16)
    WC_IMPL_BASIC_CAST(CreateTrunc, Int8)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt64)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt32)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt16)
    WC_IMPL_BASIC_CAST(CreateTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint8
//-----------------------------------------------------------------------------
class CodegenCastFromUInt8UnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromUInt8UnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CAST(CreateZExt, Int128)
    WC_IMPL_BASIC_CAST(CreateZExt, Int64)
    WC_IMPL_BASIC_CAST(CreateZExt, Int32)
    WC_IMPL_BASIC_CAST(CreateZExt, Int16)
    WC_IMPL_BASIC_CAST(CreateBitCast, Int8)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt128)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt64)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt32)
    WC_IMPL_BASIC_CAST(CreateZExt, UInt16)
    WC_IMPL_BASIC_CAST(CreateBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: void
//-----------------------------------------------------------------------------
class CodegenCastFromVoidUnaryOp final : public CodegenCastFromTypeUnaryOp {
public:
    CodegenCastFromVoidUnaryOp(Codegen & cg, const Value & fromVal, const CompiledDataType & toTypeCDT) :
        CodegenCastFromTypeUnaryOp(cg, fromVal, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// CodegenCastUnaryOp
//-----------------------------------------------------------------------------
CodegenCastUnaryOp::CodegenCastUnaryOp(Codegen & cg,
                                       const AST::ASTNode & expr,
                                       const AST::Type & toType,
                                       bool storeResultInExpr)
:
    CodegenUnaryOp(
        cg,
        expr,
        "cast(<expr> to <type>)",
        "cast",
        storeResultInExpr
    ),
    mToType(toType)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCastUnaryOp::codegen() {
    // Codegen the data type we are casting to
    mToType.accept(mCG);
    mToTypeCDT = mCG.mCtx.popCompiledDataType();
    
    // Call base class function to codegen the source expression and kick off visiting
    // the data type visitor functions for specific type casting:
    CodegenUnaryOp::codegen();
}

/* Macro to save a lot of repetition */
#define IMPL_CAST_FROM_TYPE_FUNC(FromTypeName)\
    void CodegenCastUnaryOp::visit(const FromTypeName##DataType & dataType) {\
        /* We don't use the input to these functions */\
        WC_UNUSED_PARAM(dataType);\
        /* If we don't have a valid type to cast to then bail */\
        WC_GUARD(mToTypeCDT.isValid());\
        \
        /**
         * If the data types of the source and target expression match then do nothing - no cast required.
         * In this case just push the value back onto the codegen context to finish up.
         */\
        if (mToTypeCDT.getDataType().equals(mExprVal.mCompiledType.getDataType())) {\
            mCG.mCtx.pushValue(mExprVal);\
            return;\
        }\
        \
        /* Okay, create the helper object which will do the cast and invoke it */\
        CodegenCastFrom##FromTypeName##UnaryOp(mCG,\
                                               mExprVal,\
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
IMPL_CAST_FROM_TYPE_FUNC(UInt128)
IMPL_CAST_FROM_TYPE_FUNC(UInt16)
IMPL_CAST_FROM_TYPE_FUNC(UInt32)
IMPL_CAST_FROM_TYPE_FUNC(UInt64)
IMPL_CAST_FROM_TYPE_FUNC(UInt8)
IMPL_CAST_FROM_TYPE_FUNC(Str)
IMPL_CAST_FROM_TYPE_FUNC(Void)

/* Done with this */
#undef IMPL_CAST_FROM_FUNC

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
