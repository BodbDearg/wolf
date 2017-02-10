#include "NoRTTIOps.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

llvm::Value * NoRTTIOps::IRB::CreateInsertValue(llvm::IRBuilder<> & irbuilder,
                                                llvm::Value & aggregate,
                                                llvm::Value & value,
                                                const llvm::ArrayRef<unsigned> & indexes)
{
    return irbuilder.CreateInsertValue(&aggregate,
                                       &value,
                                       indexes);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
