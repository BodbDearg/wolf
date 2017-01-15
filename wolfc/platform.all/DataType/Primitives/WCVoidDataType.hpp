#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Similar to 'unknown' in many ways but this type is exposed to the user. A data type that represents no
 * data type, or no data.
 */
class VoidDataType final : public DataType {
public:
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isSized() const override;
    
#warning FIXME - Codegen
#if 0
    virtual llvm::AllocaInst * codegenAlloca(CodegenCtx & cgCtx,
                                             AST::ASTNode & callingNode,
                                             const std::string & instLabel) override;
#endif
    
protected:
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) override;
#endif
};

WC_END_NAMESPACE
