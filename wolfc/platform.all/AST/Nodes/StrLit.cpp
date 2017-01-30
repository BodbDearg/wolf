#include "StrLit.hpp"

#include "../ASTNodeVisitor.hpp"
#include "../ParseCtx.hpp"
#include "DataType/DataType.hpp"
#include "DataType/PrimitiveDataTypes.hpp"
#include "LinearAlloc.hpp"
#include "Module.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

bool StrLit::peek(const Token * tokenPtr) {
    return tokenPtr->type == TokenType::kStrLit;
}

StrLit * StrLit::parse(ParseCtx & parseCtx) {
    if (!peek(parseCtx.tok())) {
        parseCtx.error("Expected string literal!");
        return nullptr;
    }
    
    StrLit * intLit = WC_NEW_AST_NODE(parseCtx, StrLit, *parseCtx.tok());
    parseCtx.nextTok();
    return intLit;
}

StrLit::StrLit(const Token & token) : mToken(token) {
    WC_EMPTY_FUNC_BODY();
}

void StrLit::accept(ASTNodeVisitor & visitor) const {
    visitor.visit(*this);
}

const Token & StrLit::getStartToken() const {
    return mToken;
}

const Token & StrLit::getEndToken() const {
    return mToken;
}

bool StrLit::isLValue() const {
    return false;
}

bool StrLit::isConstExpr() const {
    return true;
}

const DataType & StrLit::dataType() const {
    return PrimitiveDataTypes::getUsingTypeId(DataTypeId::kStr);
}

#warning FIXME - Codegen
#if 0
llvm::Value * StrLit::codegenAddrOf(CodegenCtx & cgCtx) {
    WC_UNUSED_PARAM(cgCtx);
    compileError("Can't take the address of string literal!");
    return nullptr;
}

llvm::Value * StrLit::codegenExprEval(CodegenCtx & cgCtx) {
    // TODO: need some sort of LUT for the module so we only have unique string instances
    return cgCtx.irBuilder.CreateGlobalStringPtr(mToken.data.strVal.ptr, "StrLit");
}

llvm::Constant * StrLit::codegenExprConstEval(CodegenCtx & cgCtx) {
    // Create a constant for the string array:
    llvm::Constant * strArray = llvm::ConstantDataArray::getString(cgCtx.llvmCtx, mToken.data.strVal.ptr);
    
    // TODO: memory management here - use the linear allocator
    // TODO: need some sort of LUT for the module so we only have unique string instances
    llvm::GlobalVariable * strGlobalVar = new llvm::GlobalVariable(
        cgCtx.module.getLLVMModuleRef(),
        strArray->getType(),
        true,                                           // Is constant
        llvm::GlobalValue::PrivateLinkage,
        strArray,                                       // Initializer
        "StrLit"
    );
    
    // Return a constant expression to get a pointer to the first element of the string
    WC_GUARD(strArray, nullptr);
    llvm::ConstantInt * firstIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(cgCtx.llvmCtx), 0);
    std::vector<llvm::Constant*> indices = { firstIndex, firstIndex };
    return llvm::ConstantExpr::getGetElementPtr(strArray->getType(), strGlobalVar, indices);
}
#endif

WC_AST_END_NAMESPACE
WC_END_NAMESPACE