#include "WCArrayDataType.hpp"

#include "AST/WCPrintStmnt.hpp"
#include "WCAssert.hpp"
#include "WCCodegenCtx.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

ArrayDataType::ArrayDataType(DataType & innerType, size_t size) :
    mInnerType(innerType),
    mSize(size)
{
    // TODO: allocate the name elsewhere?
    
    // Makeup the name:
    mName.reserve(mInnerType.name().size() + 16);
    mName += mInnerType.name();
    mName += '[';
    
    {
        char buffer[32];
        std::snprintf(buffer, 32, "%zu", mSize);
        mName += buffer;
    }
    
    mName += ']';
}

ArrayDataType::~ArrayDataType() {
    // Defined here so llvm::Type can be forward referenced in the header.
    WC_EMPTY_FUNC_BODY();
}

DataTypeId ArrayDataType::getTypeId() const {
    return DataTypeId::kArray;
}

const std::string & ArrayDataType::name() const {
    return mName;
}

bool ArrayDataType::equals(const DataType & other) const {
    // Simple check, see if the addresses match:
    if (reinterpret_cast<const void*>(this) == reinterpret_cast<const void*>(&other)) {
        return true;
    }
    
    // Get the other type as an array, if it is not an array then the types do not match:
    const ArrayDataType * otherArray = dynamic_cast<const ArrayDataType*>(&other);
    WC_GUARD(otherArray, false);
    
    // See if array sizes match:
    WC_GUARD(mSize == otherArray->mSize, false);
    
    // Lastly see if the inner types match:
    return mInnerType.equals(otherArray->mInnerType);
}

bool ArrayDataType::requiresStorage() const {
    return true;
}

llvm::AllocaInst * ArrayDataType::codegenAlloca(CodegenCtx & cgCtx,
                                                ASTNode & callingNode,
                                                const std::string & instLabel)
{
    WC_GUARD_ASSERT(mInnerType.mLLVMType, nullptr);
    WC_UNUSED_PARAM(callingNode);
    return cgCtx.irBuilder.CreateAlloca(mLLVMType, nullptr, instLabel);
}

bool ArrayDataType::codegenPrintStmnt(CodegenCtx & cgCtx,
                                      const PrintStmnt & parentPrintStmnt,
                                      llvm::Constant & printfFn,
                                      llvm::Value & value) const
{
    // TODO: support printing of arrays
    WC_UNUSED_PARAM(cgCtx);
    WC_UNUSED_PARAM(printfFn);
    WC_UNUSED_PARAM(value);
    parentPrintStmnt.compileError("Type '%s' cannot be printed (yet)!", name().c_str());
    return false;
}

bool ArrayDataType::codegenLLVMType(CodegenCtx & cgCtx, ASTNode & callingNode) {
    // If the type is unknown then give out
    if (mInnerType.isUnknown()) {
        callingNode.compileError("Unable to determine datatype for array! Datatype is ambiguous or unknown!");
        return false;
    }
    
    // First generate the inner type:
    WC_GUARD(mInnerType.codegenLLVMTypeIfRequired(cgCtx, callingNode), false);
    
    // The type must be sized in order to be code generated as an array type.
    if (!mInnerType.isSized()) {
        callingNode.compileError("Can't generate array of type '%s' because the type has no size!",
                                 mInnerType.name().c_str());
        
        return false;
    }
    
    // Get the inner type llvm type, expect that to exist at this point:
    llvm::Type * innerLLVMType = mInnerType.mLLVMType;
    WC_ASSERT(innerLLVMType);
    
    // Alright, now create an array of that inner type
    mLLVMType = llvm::ArrayType::get(innerLLVMType, mSize);
    
    if (!mLLVMType) {
        issueGenericCodegenLLVMTypeError(callingNode);
        return false;
    }
    
    return true;
}

WC_END_NAMESPACE
