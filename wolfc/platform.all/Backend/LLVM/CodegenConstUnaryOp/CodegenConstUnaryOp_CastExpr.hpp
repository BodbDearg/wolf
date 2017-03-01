//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../CompiledDataType.hpp"
#include "../Constant.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class AssignExpr;
    class Type;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class ConstCodegen;

class CodegenConstCastUnaryOp {
public:
    CodegenConstCastUnaryOp(ConstCodegen & cg,
                            const AST::AssignExpr & fromExpr,
                            const AST::Type & toType);
    
    void codegen();
    
private:
    ConstCodegen &              mCG;
    const AST::AssignExpr &     mFromExpr;
    Constant                    mFromConst;
    const AST::Type &           mToType;
    CompiledDataType            mToTypeCDT;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
