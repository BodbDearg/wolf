#include "Module.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <llvm/IR/LLVMContext.h>
    #include <llvm/IR/Module.h>
WC_THIRD_PARTY_INCLUDES_END

#include "CodegenCtx.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_CODEGEN_BEGIN_NAMESPACE

Module::Module(AST::Module & astModule) :
    mASTModule(astModule)
{
    // Create the LLVM context and module
    mLLVMCtx.reset(new llvm::LLVMContext());
    mLLVMModule.reset(new llvm::Module("WolfTest", *mLLVMCtx.get()));
    
    // Create the codegen context
    mCodegenCtx.reset(new CodegenCtx(getLLVMCtxRef(), getLLVMModuleRef()));
    
    // Do code generation
    doCodegen();
}

Module::~Module() {
    // Defined here so callee doesn't need to know details of objects destroyed by std::unique_ptr...
    WC_EMPTY_FUNC_BODY();
}

bool Module::wasCodeGeneratedOk() {
    return mLLVMModule.get() != nullptr && mCodegenCtx->errorMsgs.empty();
}

void Module::dumpIRCodeToStdout() {
    WC_GUARD_ASSERT(mLLVMModule.get());
    mLLVMModule->dump();
}

void Module::doCodegen() {
    // Declare C standard library functions we require
    declareCStdLibFuncsInModule();
    
    // Codegen all decldefs
    //for (mASTModule.md)
}

void Module::declareCStdLibFuncsInModule() {
    // We expect these objects to be created at this point
    WC_ASSERT(mLLVMCtx.get());
    WC_ASSERT(mLLVMModule.get());
    
    // For convienience
    llvm::LLVMContext & llvmCtx = *mLLVMCtx.get();
    
    #define DECL_STD_C_FUNC(FuncName, ...)\
        {\
            llvm::FunctionType * FuncName##FnType = llvm::FunctionType::get(__VA_ARGS__);\
            WC_ASSERT(FuncName##FnType);\
            WC_ASSERTED_OP(mLLVMModule->getOrInsertFunction(#FuncName, FuncName##FnType));\
        }
    
    // Declare the required functions
    DECL_STD_C_FUNC(printf,
                    llvm::Type::getInt32Ty(llvmCtx),
                    llvm::ArrayRef<llvm::Type*>{
                        llvm::Type::getInt8Ty(llvmCtx)->getPointerTo()
                    },
                    true);

    DECL_STD_C_FUNC(scanf,
                    llvm::Type::getInt32Ty(llvmCtx),
                    llvm::ArrayRef<llvm::Type*>{
                        llvm::Type::getInt8Ty(llvmCtx)->getPointerTo()
                    },
                    true);

    DECL_STD_C_FUNC(getchar,
                    llvm::Type::getInt32Ty(llvmCtx),
                    llvm::ArrayRef<llvm::Type*>{},
                    false);

    DECL_STD_C_FUNC(time,
                    llvm::Type::getInt64Ty(llvmCtx),
                    llvm::ArrayRef<llvm::Type*>{
                        llvm::Type::getInt64Ty(llvmCtx)->getPointerTo(),
                    },
                    false);

    DECL_STD_C_FUNC(rand,
                    llvm::Type::getInt32Ty(llvmCtx),
                    llvm::ArrayRef<llvm::Type*>{},
                    false);

    DECL_STD_C_FUNC(srand,
                    llvm::Type::getVoidTy(llvmCtx),
                    llvm::ArrayRef<llvm::Type*>{
                        llvm::Type::getInt32Ty(llvmCtx),
                    },
                    false);

    DECL_STD_C_FUNC(abort,
                    llvm::Type::getVoidTy(llvmCtx),
                    llvm::ArrayRef<llvm::Type*>{},
                    false);
}

WC_LLVM_CODEGEN_END_NAMESPACE
WC_END_NAMESPACE
