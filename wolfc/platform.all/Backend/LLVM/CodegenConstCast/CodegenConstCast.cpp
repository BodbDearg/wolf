//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstCast.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "Assert.hpp"
#include "DataType/Types/BoolDataType.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------
// Abstract base class that converts from one type at compile time to many
// other types at compile time.
//-----------------------------------------------------------------------------
class CodegenConstCastFromDataType : public DataTypeVisitor {
public:
    CodegenConstCastFromDataType(ConstCodegen & cg,
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
    
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Any)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Array)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Bool)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Func)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int128)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int16)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int32)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int64)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Int8)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Nullptr)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Ptr)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Str)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt128)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt16)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt32)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt64)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(UInt8)
    IMPL_UNSUPPORTED_CAST_OP_FUNC(Undefined)
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
    /* Spit out an error that a cast is not supported */
    void issueUnsupportedCastError() const {
        mCG.mCtx.error("Illegal cast! Cannot do a compile time cast from an expression of type '%s' to type '%s'!",
                       mFromConst.mCompiledType.getDataType().name().c_str(),
                       mToTypeCDT.getDataType().name().c_str());
    }
    
    ConstCodegen &              mCG;
    const Constant &            mFromConst;
    const CompiledDataType &    mToTypeCDT;
};

/* This macro saves a lot of repetition */
#define WC_IMPL_BASIC_CONST_CAST(CastGetFunc, ToDataTypeName)\
    virtual void visit(const ToDataTypeName##DataType & toType) override {\
        WC_UNUSED_PARAM(toType);\
        WC_ASSERT(mFromConst.mLLVMConst);\
        WC_ASSERT(mToTypeCDT.getLLVMType());\
        pushOpResult(llvm::ConstantExpr::CastGetFunc(mFromConst.mLLVMConst, mToTypeCDT.getLLVMType()));\
    }

//-----------------------------------------------------------------------------
// Cast From: any
//-----------------------------------------------------------------------------
class CodegenConstCastFromAnyDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromAnyDataType(ConstCodegen & cg,
                                    const Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: Array
//-----------------------------------------------------------------------------
class CodegenConstCastFromArrayDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromArrayDataType(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: bool
//-----------------------------------------------------------------------------
class CodegenConstCastFromBoolDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromBoolDataType(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: Func
//-----------------------------------------------------------------------------
class CodegenConstCastFromFuncDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromFuncDataType(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: int128
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt128DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromInt128DataType(ConstCodegen & cg,
                                       const Constant & fromConst,
                                       const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int128)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int16
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt16DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromInt16DataType(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int32)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int32
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt32DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromInt32DataType(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int64
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt64DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromInt64DataType(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: int8
//-----------------------------------------------------------------------------
class CodegenConstCastFromInt8DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromInt8DataType(ConstCodegen & cg,
                                     const Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int32)
    WC_IMPL_BASIC_CONST_CAST(getSExt, Int16)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getSExt, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: nullptr
//-----------------------------------------------------------------------------
class CodegenConstCastFromNullptrDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromNullptrDataType(ConstCodegen & cg,
                                        const Constant & fromConst,
                                        const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int16)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int32)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int8)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt8)
    
    virtual void visit(const BoolDataType & dataType) override {
        // 'null' will always equate to 'false' so just fastrack that comparison here
        WC_UNUSED_PARAM(dataType);
        llvm::Constant * falseConst = llvm::ConstantInt::get(llvm::Type::getInt1Ty(mCG.mCtx.mLLVMCtx), 0);
        WC_ASSERT(falseConst);
        mCG.mCtx.pushConstant(Constant(falseConst, mToTypeCDT, mCG.mCtx.getCurrentASTNode()));
    }
    
    virtual void visit(const PtrDataType & toType) override {
        // Expect these to be valid
        WC_ASSERT(mToTypeCDT.isValid());
        WC_ASSERT(mFromConst.mLLVMConst);
        
        // Only allow a cast to the pointer data type if the pointer type is not null.
        // Can't cast 'null' to a non nullable pointer type.
        if (toType.mIsNullable) {
            pushOpResult(llvm::ConstantExpr::getPointerCast(mFromConst.mLLVMConst, mToTypeCDT.getLLVMType()));
        }
        else {
            mCG.mCtx.error("Casting the 'null' literal to a non nullable pointer of type '%s' is illegal! "
                           "Non nullable pointers are not allowed to be null in well defined code.",
                           toType.mName.c_str());
        }
    }
};

//-----------------------------------------------------------------------------
// Cast From: Ptr
//-----------------------------------------------------------------------------
class CodegenConstCastFromPtrDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromPtrDataType(ConstCodegen & cg,
                                    const Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int16)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int32)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, Int8)
    WC_IMPL_BASIC_CONST_CAST(getPointerCast, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getPtrToInt, UInt8)
    
    virtual void visit(const BoolDataType & dataType) override {
        // Expect this to be valid
        WC_ASSERT(mFromConst.mLLVMConst);
        
        // Doing this for a non nullable pointer type is an error:
        const PtrDataType & fromPtrType = static_cast<const PtrDataType&>(mFromConst.mCompiledType.getDataType());
        
        if (!fromPtrType.mIsNullable) {
            mCG.mCtx.error("Attempting to cast a non nullable pointer of type '%s' to type '%s' via a pointer "
                           "null check! Non nullable pointers can never be null in well defined code, hence "
                           "checking if they are null and converting to bool based on that result is illegal!",
                           fromPtrType.name().c_str(),
                           dataType.name().c_str());
            
            return;
        }
        
        // If the value is non null push a true value, otherwise push false
        llvm::Constant * boolConst = mFromConst.mLLVMConst->isNullValue() ?
            llvm::ConstantInt::get(llvm::Type::getInt1Ty(mCG.mCtx.mLLVMCtx), 0) :
            llvm::ConstantInt::get(llvm::Type::getInt1Ty(mCG.mCtx.mLLVMCtx), 1);
        
        WC_ASSERT(boolConst);
        mCG.mCtx.pushConstant(Constant(boolConst, mToTypeCDT, mCG.mCtx.getCurrentASTNode()));
    }
};

//-----------------------------------------------------------------------------
// Cast From: str
//-----------------------------------------------------------------------------
class CodegenConstCastFromStrDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromStrDataType(ConstCodegen & cg,
                                    const Constant & fromConst,
                                    const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: uint128
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt128DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromUInt128DataType(ConstCodegen & cg,
                                        const Constant & fromConst,
                                        const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int128)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint16
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt16DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromUInt16DataType(ConstCodegen & cg,
                                       const Constant & fromConst,
                                       const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int32)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint32
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt32DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromUInt32DataType(ConstCodegen & cg,
                                       const Constant & fromConst,
                                       const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint64
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt64DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromUInt64DataType(ConstCodegen & cg,
                                       const Constant & fromConst,
                                       const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getTrunc, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: uint8
//-----------------------------------------------------------------------------
class CodegenConstCastFromUInt8DataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromUInt8DataType(ConstCodegen & cg,
                                      const Constant & fromConst,
                                      const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
    
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int128)
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int64)
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int32)
    WC_IMPL_BASIC_CONST_CAST(getZExt, Int16)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, Int8)
    WC_IMPL_BASIC_CONST_CAST(getIntToPtr, Ptr)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt128)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt64)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt32)
    WC_IMPL_BASIC_CONST_CAST(getZExt, UInt16)
    WC_IMPL_BASIC_CONST_CAST(getBitCast, UInt8)
};

//-----------------------------------------------------------------------------
// Cast From: <undefined>
//-----------------------------------------------------------------------------
class CodegenConstCastFromUndefinedDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromUndefinedDataType(ConstCodegen & cg,
                                          const Constant & fromConst,
                                          const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// Cast From: void
//-----------------------------------------------------------------------------
class CodegenConstCastFromVoidDataType final : public CodegenConstCastFromDataType {
public:
    CodegenConstCastFromVoidDataType(ConstCodegen & cg, const
                                     Constant & fromConst,
                                     const CompiledDataType & toTypeCDT)
    :
        CodegenConstCastFromDataType(cg, fromConst, toTypeCDT)
    {
        WC_EMPTY_FUNC_BODY();
    }
};

//-----------------------------------------------------------------------------
// CodegenConstCast
//-----------------------------------------------------------------------------
CodegenConstCast::CodegenConstCast(ConstCodegen & cg,
                                   const Constant & fromConst,
                                   const CompiledDataType & toTypeCDT)
:
    mCG(cg),
    mFromConst(fromConst),
    mToTypeCDT(toTypeCDT)
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstCast::codegen() {
    // If the value we are casting is invalid then bail:
    if (!mFromConst.isValid()) {
        mCG.mCtx.error("Can't code generate a compile time cast from an invalid value of type '%s'!",
                       mFromConst.mCompiledType.getDataType().name().c_str());
        
        return;
    }
    
    // If the type we are casting to is invalid then bail:
    if (!mToTypeCDT.isValid()) {
        mCG.mCtx.error("Can't code generate a compile time cast to invalid data type '%s'!",
                       mToTypeCDT.getDataType().name().c_str());
        
        return;
    }
    
    // If the data types of the source and target expression match then do nothing - no cast required.
    // In this case just push the constant back onto the codegen context to finish up.
    if (mToTypeCDT.getDataType().equals(mFromConst.mCompiledType.getDataType())) {
        mCG.mCtx.pushConstant(mFromConst);
        return;
    }
    
    // Otherwise codegen the cast:
    mFromConst.mCompiledType.getDataType().accept(*this);
}

/* Macro to save a lot of repetition */
#define IMPL_CAST_FROM_TYPE_FUNC(FromTypeName)\
    void CodegenConstCast::visit(const FromTypeName##DataType & dataType) {\
        /* We don't use the input to these functions */\
        WC_UNUSED_PARAM(dataType);\
        /* Sanity check, we shouldn't get to here if these conditions are not true! */\
        WC_ASSERT(mFromConst.isValid());\
        WC_ASSERT(mToTypeCDT.isValid());\
        \
        /* Okay, create the helper object which will do the cast and invoke it */\
        CodegenConstCastFrom##FromTypeName##DataType(mCG,\
                                                     mFromConst,\
                                                     mToTypeCDT).codegen();\
    }

IMPL_CAST_FROM_TYPE_FUNC(Any)
IMPL_CAST_FROM_TYPE_FUNC(Array)
IMPL_CAST_FROM_TYPE_FUNC(Bool)
IMPL_CAST_FROM_TYPE_FUNC(Func)
IMPL_CAST_FROM_TYPE_FUNC(Int128)
IMPL_CAST_FROM_TYPE_FUNC(Int16)
IMPL_CAST_FROM_TYPE_FUNC(Int32)
IMPL_CAST_FROM_TYPE_FUNC(Int64)
IMPL_CAST_FROM_TYPE_FUNC(Int8)
IMPL_CAST_FROM_TYPE_FUNC(Nullptr)
IMPL_CAST_FROM_TYPE_FUNC(Ptr)
IMPL_CAST_FROM_TYPE_FUNC(Str)
IMPL_CAST_FROM_TYPE_FUNC(UInt128)
IMPL_CAST_FROM_TYPE_FUNC(UInt16)
IMPL_CAST_FROM_TYPE_FUNC(UInt32)
IMPL_CAST_FROM_TYPE_FUNC(UInt64)
IMPL_CAST_FROM_TYPE_FUNC(UInt8)
IMPL_CAST_FROM_TYPE_FUNC(Undefined)
IMPL_CAST_FROM_TYPE_FUNC(Void)

/* Done with this */
#undef IMPL_CAST_FROM_FUNC

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
