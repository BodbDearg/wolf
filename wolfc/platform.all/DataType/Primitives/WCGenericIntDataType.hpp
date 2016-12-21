#pragma once

#include "DataType/WCDataType.hpp"

WC_BEGIN_NAMESPACE

/* Base class for all integer data types */
class GenericIntDataType : public DataType {
public:
    virtual bool isInteger() const final override;
    
    virtual llvm::Value * codegenAddOp(CodegenCtx & cgCtx,
                                       ASTNode & callingNode,
                                       llvm::Value & leftVal,
                                       DataType & rightType,
                                       llvm::Value & rightVal) override;
};

WC_END_NAMESPACE
