//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AddrOrValCodegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "AST/Nodes/AddExpr.hpp"
#include "AST/Nodes/ArrayLit.hpp"
#include "AST/Nodes/AssertStmnt.hpp"
#include "AST/Nodes/AssignExpr.hpp"
#include "AST/Nodes/BoolLit.hpp"
#include "AST/Nodes/BreakStmnt.hpp"
#include "AST/Nodes/CastExpr.hpp"
#include "AST/Nodes/CmpExpr.hpp"
#include "AST/Nodes/DeclDef.hpp"
#include "AST/Nodes/Func.hpp"
#include "AST/Nodes/FuncArg.hpp"
#include "AST/Nodes/Identifier.hpp"
#include "AST/Nodes/IfStmnt.hpp"
#include "AST/Nodes/IntLit.hpp"
#include "AST/Nodes/LAndExpr.hpp"
#include "AST/Nodes/LOrExpr.hpp"
#include "AST/Nodes/LoopStmnt.hpp"
#include "AST/Nodes/Module.hpp"
#include "AST/Nodes/MulExpr.hpp"
#include "AST/Nodes/NextStmnt.hpp"
#include "AST/Nodes/NoOpStmnt.hpp"
#include "AST/Nodes/NotExpr.hpp"
#include "AST/Nodes/NullLit.hpp"
#include "AST/Nodes/PostfixExpr.hpp"
#include "AST/Nodes/PrefixExpr.hpp"
#include "AST/Nodes/PrimaryExpr.hpp"
#include "AST/Nodes/PrimitiveType.hpp"
#include "AST/Nodes/PrintStmnt.hpp"
#include "AST/Nodes/RandExpr.hpp"
#include "AST/Nodes/ReadnumExpr.hpp"
#include "AST/Nodes/ReturnStmnt.hpp"
#include "AST/Nodes/Scope.hpp"
#include "AST/Nodes/ScopeStmnt.hpp"
#include "AST/Nodes/ShiftExpr.hpp"
#include "AST/Nodes/Stmnt.hpp"
#include "AST/Nodes/StrLit.hpp"
#include "AST/Nodes/TernaryExpr.hpp"
#include "AST/Nodes/TimeExpr.hpp"
#include "AST/Nodes/Type.hpp"
#include "AST/Nodes/VarDecl.hpp"
#include "AST/Nodes/WhileStmnt.hpp"
#include "DataType/Types/ArrayDataType.hpp"
#include "DataType/Types/PtrDataType.hpp"

WC_BEGIN_NAMESPACE
WC_LLVM_BACKEND_BEGIN_NAMESPACE

AddrOrValCodegen::AddrOrValCodegen(CodegenCtx & ctx,
                                   Codegen & codegen,
                                   AddrCodegen & addrCodegen)
:
    mCtx(ctx),
    mCodegen(codegen),
    mAddrCodegen(addrCodegen)
{
    WC_EMPTY_FUNC_BODY();
}

/* Nodes we should never expect to call this on */
#define WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(NodeType)\
    void AddrOrValCodegen::visit(const AST::NodeType & astNode) {\
        WC_CODEGEN_RECORD_VISITED_NODE();\
        codegenNotSupportedForNodeTypeError(astNode, #NodeType);\
    }

WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(AssertStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(BreakStmntNoCond)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(BreakStmntWithCond)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(DeclDefFunc)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(DeclDefVarDecl)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(Func)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(FuncArg)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(IfStmntElse)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(IfStmntElseIf)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(IfStmntNoElse)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(LoopStmntNoCond)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(LoopStmntWithCond)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(Module)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(NextStmntNoCond)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(NextStmntWithCond)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(NoOpStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(PrimitiveType)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(PrintStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(ReturnStmntNoCondVoid)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(ReturnStmntNoCondWithValue)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(ReturnStmntWithCondAndValue)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(ReturnStmntWithCondVoid)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(Scope)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(ScopeStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntAssertStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntBreakStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntIfStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntLoopStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntNextStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntNoOpStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntPrintStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntReturnStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntScopeStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntVarDecl)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(StmntWhileStmnt)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(TypeArray)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(TypePrimitive)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(TypePtr)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(VarDeclExplicitType)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(VarDeclInferType)
WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(WhileStmnt)

#undef WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE

/* Nodes that delegate to the regular code generator to generate a value */
#define WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(NodeType)\
    void AddrOrValCodegen::visit(const AST::NodeType & astNode) {\
        astNode.accept(mCodegen);\
    }

WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AddExprAdd)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AddExprBOr)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AddExprBXor)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AddExprSub)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssign)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignARShift)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignAdd)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignBAnd)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignBOr)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignBXor)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignDiv)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignLRShift)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignLShift)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignMul)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignRem)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(AssignExprAssignSub)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(BoolLit)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(CastExprCast)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(CmpExprEQ)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(CmpExprGE)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(CmpExprGT)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(CmpExprLE)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(CmpExprLT)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(CmpExprNE)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(IntLit)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(LAndExprAnd)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(LOrExprOr)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(MulExprBAnd)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(MulExprDiv)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(MulExprMul)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(MulExprRem)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(NotExprBNot)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(NotExprLNot)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(NullLit)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(PostfixExprDec)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(PostfixExprInc)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(PrefixExprAddrOf)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(PrefixExprMinus)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(PrefixExprPlus)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(PrefixExprPtrDenull)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(RandExprRand)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(RandExprSRand)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(ReadnumExpr)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(ShiftExprARShift)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(ShiftExprLRShift)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(ShiftExprLShift)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(StrLit)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(TernaryExprWithCond)
WC_IMPL_CODEGEN_VALUE_FOR_NODE_TYPE(TimeExpr)

#undef WC_IMPL_VALUE_CODEGEN_FOR_NODE_TYPE

/* Nodes that delegate to the address code generator to generate an address */
#define WC_IMPL_CODEGEN_ADDR_FOR_NODE_TYPE(NodeType)\
    void AddrOrValCodegen::visit(const AST::NodeType & astNode) {\
        astNode.accept(mAddrCodegen);\
    }

WC_IMPL_CODEGEN_ADDR_FOR_NODE_TYPE(ArrayLit)
WC_IMPL_CODEGEN_ADDR_FOR_NODE_TYPE(Identifier)
WC_IMPL_CODEGEN_ADDR_FOR_NODE_TYPE(PostfixExprArrayLookup)
WC_IMPL_CODEGEN_ADDR_FOR_NODE_TYPE(PrefixExprPtrDeref)

#undef WC_IMPL_CODEGEN_ADDR_FOR_NODE_TYPE

/* These expression nodes delegate the query to sub expressions */
#define WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(NodeType, subExprField)\
    void AddrOrValCodegen::visit(const AST::NodeType & astNode) {\
        WC_CODEGEN_RECORD_VISITED_NODE();\
        astNode.subExprField.accept(*this);\
    }

WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(AddExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(AssignExprNoAssign, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(CastExprNoCast, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(CmpExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(LAndExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(LOrExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(MulExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(NotExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PostfixExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrefixExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprArrayLit, mLit)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprBoolLit, mLit)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprIdentifier, mIdent)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprIntLit, mLit)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprNullLit, mLit)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprParen, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprRandExpr, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprReadnumExpr, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprStrLit, mLit)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(PrimaryExprTimeExpr, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(ShiftExprNoOp, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(StmntAssignExpr, mExpr)
WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(TernaryExprNoCond, mExpr)

#undef WC_IMPL_DELEGATE_OP_TO_INNER_EXPR_FOR_NODE_TYPE

void AddrOrValCodegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    // Evaluate the expression first:
    astNode.accept(mCodegen);
    Value exprVal = mCtx.popValue();
    
    // The value must be valid for this to work
    WC_GUARD(exprVal.isValid());
    
    // If the value requires storage, allocate the storage on the stack and return a pointer to that storage.
    // Otherwise return the raw value itself:
    const CompiledDataType & exprValCDT = exprVal.mCompiledType;
    const DataType & exprValDT = exprValCDT.getDataType();
    
    if (exprValDT.requiresStorage()) {
        // Dealing with a return type which requires storage.
        // Create an alloca to hold the result of the function call and store the result there.
        llvm::Value * llvmStackVal = mCtx.mIRBuilder.CreateAlloca(exprValCDT.getLLVMType(),
                                                                  nullptr,
                                                                  "AddrOrValCodegen:PostfixExprFuncCall:Alloca");
        
        WC_ASSERT(llvmStackVal);
        mCtx.mIRBuilder.CreateStore(exprVal.mLLVMVal, llvmStackVal);
        
        // Push it onto the codegen context stack and mark it as requring a load
        mCtx.pushValue(Value(llvmStackVal, exprValCDT, true, &astNode));
    }
    else {
        // Return type which doesn't require any storage. Just return the value as-is:
        mCtx.pushValue(exprVal);
    }
}

void AddrOrValCodegen::codegenNotSupportedForNodeTypeError(const AST::ASTNode & node,
                                                           const char * nodeClassName)
{
    mCtx.error(node,
               "Code generating the address or value of AST node type '%s' is not supported!",
               nodeClassName);
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
