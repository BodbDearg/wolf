#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * String data type.
 * TODO: will probably change once we have a string class and once we support pointers to chars.
 */
class StrDataType final : public DataType {
public:
    virtual DataTypeId getTypeId() const override;
    virtual const std::string & name() const override;
    virtual bool equals(const DataType & other) const override;

#warning FIXME - Codegen
#if 0
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   AST::ASTNode & callingNode,
                                   llvm::Constant & printfFn,
                                   llvm::Value & valToPrint) override;
#endif
    
protected:
    
#warning FIXME - Codegen
#if 0
    virtual bool codegenLLVMType(CodegenCtx & cgCtx, AST::ASTNode & callingNode) override;
#endif
};

WC_END_NAMESPACE
