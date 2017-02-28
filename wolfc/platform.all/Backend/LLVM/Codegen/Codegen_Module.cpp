#include "Codegen.hpp"

#include "../CodegenCtx.hpp"
#include "AST/Nodes/DeclDef.hpp"
#include "AST/Nodes/Module.hpp"
#include "Assert.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

/**
 * Declare all the standard C functions required by codegen
 * in the given llvm module.
 */
static void declareCStdLibFuncsInModule(llvm::LLVMContext & llvmCtx, llvm::Module & llvmModule) {
    #define DECL_STD_C_FUNC(FuncName, ...)\
        {\
            llvm::FunctionType * FuncName##FnType = llvm::FunctionType::get(__VA_ARGS__);\
            WC_ASSERT(FuncName##FnType);\
            WC_ASSERTED_OP(llvmModule.getOrInsertFunction(#FuncName, FuncName##FnType));\
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

void Codegen::visit(const AST::Module & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Create the LLVM module
    mCtx.mLLVMModule.reset(new llvm::Module(mCtx.getModuleName(), mCtx.mLLVMCtx));
    WC_ASSERT(mCtx.mLLVMModule);
    
    // Add required standard C functions to the module
    declareCStdLibFuncsInModule(mCtx.mLLVMCtx, *mCtx.mLLVMModule.get());
    
    // Codegen all stuff in the module:
    for (const AST::DeclDef * declDef : astNode.mDeclDefs) {
        declDef->accept(*this);
    }
    
    // Do any deferred codegen required:
    mCtx.handleDeferredCodegenCallbacks(mCtx.mDeferredCgCallbacks_Module);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
