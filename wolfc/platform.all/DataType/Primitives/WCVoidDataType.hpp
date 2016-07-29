#pragma once

#include "WCDataType.hpp"

WC_BEGIN_NAMESPACE

/**
 * Similar to 'unknown' in many ways but this type is exposed to the user. A data type that represents no
 * data type, or no data.
 */
class VoidDataType : public DataType {
public:
    virtual const std::string & name() const override;
    
    virtual bool equals(const DataType & other) const override;
    
    virtual bool isSized() const override;
    
    virtual bool isVoid() const override;
    
    virtual bool codegen(CodegenCtx & cgCtx, ASTNode & callingNode) override;
    
    virtual bool codegenPrintStmnt(CodegenCtx & cgCtx,
                                   const PrintStmnt & parentPrintStmnt,
                                   llvm::Constant & printfFn,
                                   llvm::Value & value) const override;
};

WC_END_NAMESPACE
