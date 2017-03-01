//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "../CompiledDataType.hpp"
#include "../Value.hpp"

WC_BEGIN_NAMESPACE

namespace AST {
    class AssignExpr;
    class Type;
}

WC_LLVM_BACKEND_BEGIN_NAMESPACE

class Codegen;

class CodegenCastUnaryOp {
public:
    CodegenCastUnaryOp(Codegen & cg,
                       const AST::AssignExpr & fromExpr,
                       const AST::Type & toType);
    
    void codegen();
    
private:
    Codegen &                   mCG;
    const AST::AssignExpr &     mFromExpr;
    Value                       mFromVal;
    const AST::Type &           mToType;
    CompiledDataType            mToTypeCDT;
};

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
