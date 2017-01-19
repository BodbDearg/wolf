#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Data type that represents an unknown type. Cannot be declared by the user. Used internally by the
 * compiler in the case of an error.
 */
class UnknownDataType final : public DataType {
public:
    virtual void accept(DataTypeVisitor & visitor) const override;
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;
    virtual bool isValid() const override;
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
