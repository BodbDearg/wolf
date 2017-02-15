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
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int64)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Invalid)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Str)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Void)
    
    /* Done with this now! */
    #undef IMPL_UNSUPPORTED_CAST_OP_FUNC
    
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
IMPL_CAST_FROM_TYPE_FUNC(Int64)
IMPL_CAST_FROM_TYPE_FUNC(Str)
IMPL_CAST_FROM_TYPE_FUNC(Void)

/* Done with this */
#undef IMPL_CAST_FROM_FUNC

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
