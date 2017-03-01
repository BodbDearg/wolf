//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "CodegenConstUnaryOp_CastExpr.hpp"

#include "../CodegenConstCast/CodegenConstCast.hpp"
#include "../CodegenCtx.hpp"
#include "../ConstCodegen/ConstCodegen.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/Type.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

CodegenConstCastUnaryOp::CodegenConstCastUnaryOp(ConstCodegen & cg,
                                                 const AST::AssignExpr & fromExpr,
                                                 const AST::Type & toType)
:
    mCG(cg),
    mFromExpr(fromExpr),
    mFromConst(),
    mToType(toType),
    mToTypeCDT()
{
    WC_EMPTY_FUNC_BODY();
}

void CodegenConstCastUnaryOp::codegen() {
    // Evaluate the expression being casted from
    mFromExpr.accept(mCG);
    mFromConst = mCG.mCtx.popConstant();
    
    // Codegen the data type we are casting to
    mToType.accept(mCG);
    mToTypeCDT = mCG.mCtx.popCompiledDataType();
    
    // Only if both are valid, proceed:
    WC_GUARD(mFromConst.isValid() && mToTypeCDT.isValid());
    
    // Do the cast!
    CodegenConstCast(mCG, mFromConst, mToTypeCDT).codegen();
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
