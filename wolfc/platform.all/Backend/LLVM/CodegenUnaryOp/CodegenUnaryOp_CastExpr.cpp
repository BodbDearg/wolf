//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenUnaryOp_CastExpr.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCast/CodegenCast.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/Type.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenCastUnaryOp::CodegenCastUnaryOp(Codegen & cg,
                                       const AST::AssignExpr & fromExpr,
                                       const AST::Type & toType)
:
    mCG(cg),
    mFromExpr(fromExpr),
    mFromVal(),
    mToType(toType),
    mToTypeCDT()
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenCastUnaryOp::codegen() {
    // Evaluate the expression being casted from
    mFromExpr.accept(mCG);
    mFromVal = mCG.mCtx.popValue();
    
    // Codegen the data type we are casting to
    mToType.accept(mCG);
    mToTypeCDT = mCG.mCtx.popCompiledDataType();
    
    // Only if both are valid, proceed:
    WC_GUARD(mFromVal.isValid() && mToTypeCDT.isValid());
    
    // Do the cast!
    CodegenCast(mCG, mFromVal, mToTypeCDT).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
