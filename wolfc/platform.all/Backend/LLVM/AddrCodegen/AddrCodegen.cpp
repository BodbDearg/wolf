//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#include "AddrCodegen.hpp"

#include "../Codegen/Codegen.hpp"
#include "../CodegenCtx.hpp"
#include "../CodegenUnaryOp/CodegenUnaryOp_PrefixExpr.hpp"
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

AddrCodegen::AddrCodegen(CodegenCtx & ctx,
                         Codegen & codegen,
                         ConstCodegen & constCodegen,
                         CodegenDataType & codegenDataType)
:
    mCtx(ctx),
    mCodegen(codegen),
    mConstCodegen(constCodegen),
    mCodegenDataType(codegenDataType),
    mAddrOrValCodegen(ctx, codegen, *this)
{
    WC_EMPTY_FUNC_BODY();
}

/* Nodes we should never expect to call this on */
#define WC_IMPL_OP_UNSUPPORTED_FOR_NODE_TYPE(NodeType)\
    void AddrCodegen::visit(const AST::NodeType & astNode) {\
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

/* Binary operations we can't take the address of */
#define WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(NodeType, opSymbol, opName)\
    void AddrCodegen::visit(const AST::NodeType & astNode) {\
        WC_CODEGEN_RECORD_VISITED_NODE();\
        cantTakeAddressOfBinaryOpError(astNode.mLeftExpr,\
                                       astNode.mRightExpr,\
                                       opSymbol,\
                                       opName);\
    }

WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(AddExprAdd, "+", "add")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(AddExprBOr, "|", "bitwise or")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(AddExprBXor, "^", "bitwise xor")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(AddExprSub, "-", "subtract")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(CmpExprEQ, "==", "equals")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(CmpExprGE, ">=", "greater than or equal to")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(CmpExprGT, ">", "greater than")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(CmpExprLE, "<=", "less than or equal to")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(CmpExprLT, "<", "less than")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(CmpExprNE, "!=", "not equals")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(LAndExprAnd, "and", "logical and")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(LOrExprOr, "or", "logical or")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(MulExprBAnd, "&", "bitwise and")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(MulExprDiv, "/", "divide")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(MulExprMul, "*", "multiply")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(MulExprRem, "%", "remainder")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(ShiftExprARShift, ">>", "arithmetic right shift")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(ShiftExprLRShift, ">>>", "logical right shift")
WC_IMPL_CANT_TAKE_BINARY_OP_ADDR_FOR_NODE_TYPE(ShiftExprLShift, "<<", "left shift")

/* Unary operations we can't take the address of */
#define WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(NodeType, opSymbol, opName)\
    void AddrCodegen::visit(const AST::NodeType & astNode) {\
        WC_CODEGEN_RECORD_VISITED_NODE();\
        cantTakeAddressOfUnaryOpError(astNode, opSymbol, opName);\
    }

WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(NotExprBNot, "~", "bitwise not")
WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(NotExprLNot, "not", "logical not")
WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(PostfixExprDec, "--", "decrement")
WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(PostfixExprInc, "++", "increment")
WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(PrefixExprAddrOf, "&", "address of")
WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(PrefixExprMinus, "-", "minus")
WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE(PrefixExprPlus, "+", "plus")

#undef WC_IMPL_CANT_TAKE_UNARY_OP_ADDR_FOR_NODE_TYPE

/* Assign expressions we can't take the address of */
#define WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(NodeType)\
    void AddrCodegen::visit(const AST::NodeType & astNode) {\
        WC_CODEGEN_RECORD_VISITED_NODE();\
        mCtx.error(astNode, "Can't take the address of an assign expression!");\
    }

WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssign)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignARShift)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignAdd)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignBAnd)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignBOr)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignBXor)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignDiv)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignLRShift)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignLShift)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignMul)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignRem)
WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE(AssignExprAssignSub)

#undef WC_IMPL_CANT_TAKE_ASSIGN_EXPR_ADDR_FOR_NODE_TYPE

/* General node types we can't take the address of */
#define WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(NodeType, msg)\
    void AddrCodegen::visit(const AST::NodeType & astNode) {\
        WC_CODEGEN_RECORD_VISITED_NODE();\
        mCtx.error(astNode, msg);\
    }

WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(BoolLit, "Can't take the address of a bool literal!")
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(IntLit, "Can't take the address of a integer literal!")
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(NullLit, "Can't take the address of a 'null' pointer literal!")
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(RandExprRand, "Can't take the address of a rand() expression!")
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(RandExprSRand, "Can't take the address of a srand() expression!")
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(ReadnumExpr, "Can't take the address of a readnum() expression!")
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(StrLit, "Can't take the address of a string literal!")
#warning TODO: Investigate what C does for this, feels like we shouldn't be able to do though
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(TernaryExprWithCond, "Can't take the address of a ternary expression!")
WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE(TimeExpr, "Can't take the address of a time() expression!")

#undef WC_IMPL_CANT_TAKE_ADDR_FOR_NODE_TYPE

/* These expression nodes delegate the query to sub expressions */
#define WC_IMPL_DELEGATE_OP_TO_SUB_EXPR_FOR_NODE_TYPE(NodeType, subExprField)\
    void AddrCodegen::visit(const AST::NodeType & astNode) {\
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

void AddrCodegen::visit(const AST::ArrayLit & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codegen the literal
    astNode.accept(mCodegen);
    Value exprVal = mCtx.popValue();
    
    // The value must be valid for this to work
    WC_GUARD(exprVal.isValid());
    
    // Create an alloca to hold the literal on the stack so we can take it's address
    const CompiledDataType & exprValCDT = exprVal.mCompiledType;
    llvm::Value * llvmStackVal = mCtx.mIRBuilder.CreateAlloca(exprValCDT.getLLVMType(),
                                                              nullptr,
                                                              "AddrCodegen:ArrayLit:Alloca");
    
    WC_ASSERT(llvmStackVal);
    mCtx.mIRBuilder.CreateStore(exprVal.mLLVMVal, llvmStackVal);
    
    // Push it onto the codegen context stack
    mCtx.pushValue(Value(llvmStackVal, exprValCDT, true, &astNode));
}

void AddrCodegen::visit(const AST::CastExprCast & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Get the value (and hence the data type) being from:
    astNode.mExpr.accept(mCodegen);
    Value exprVal = mCtx.popValue();
    
    // Get the type being casted to:
    astNode.mType.accept(*this);
    CompiledDataType toType = mCtx.popCompiledDataType();
    
    // Issue the error
    mCtx.error(astNode,
               "Can't take the address of a cast operation casting from "
               "type '%s' to type '%s'!",
               exprVal.mCompiledType.getDataType().name().c_str(),
               toType.getDataType().name().c_str());
}

void AddrCodegen::visit(const AST::Identifier & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Search for a value declared in the current scope or parent scopes.
    // When found just return the value, variables are stored by address.
    const Value * value = mCtx.lookupValueByName(astNode.name());
    
    if (value) {
        mCtx.pushValue(*value);
        return;
    }
    
    // Couldn't find any value with this name, issue a compile error:
    mCtx.error(astNode,
               "No symbol named '%s' (variable, function etc.) in the current scope! "
               "Can't lookup the address of this symbol!",
               astNode.name());
}

void AddrCodegen::visit(const AST::PostfixExprArrayLookup & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    
    // Codgen either the address or value of the 'array' expression first, depending on whether we are
    // dealing with an addressable expression (l-value) or not.
    //
    // Note: despite the terminology the array expresion might not neccesarily be an array, it could also
    // potentially be any other type that we support indexing on (such as pointers).
    astNode.mArrayExpr.accept(mAddrOrValCodegen);
    Value arrayExprVal = mCtx.popValue();
    
    // Codegen the expression for the array index
    astNode.mIndexExpr.accept(mCodegen);
    Value indexVal = mCtx.popValue();
    WC_ASSERT(!indexVal.isValid() || !indexVal.mRequiresLoad);
    
    // Figure out if we can do array indexing on the type being indexed and get the element
    // type if indexing is possible.
    //
    // TODO: support the array lookup operator on custom types eventually.
    const CompiledDataType & arrayExprValCDT = arrayExprVal.mCompiledType;
    const DataType & arrayExprValDT = arrayExprValCDT.getDataType();
    const DataType * arrayElemDT = nullptr;
    bool exprTypeAndIndexTypesAreOk = true;
    
    if (arrayExprValDT.isArray()) {
        // Indexing an array
        const ArrayDataType & arrayDT = static_cast<const ArrayDataType&>(arrayExprValDT);
        arrayElemDT = &arrayDT.mElemType;
        
        // Arrays should always require a load
        WC_ASSERT(arrayExprVal.mRequiresLoad);
    }
    else if (arrayExprValDT.isPtr()) {
        // Indexing a pointer, similar to how it is done in the 'C' language.
        //
        // Note: pointers may or may not require a load, depending on whether we accessed them
        // through an identifier (l-value) or some sort of r-value. Hence, we do no assert check in
        // this case on whether the value should be loaded or not...
        const PtrDataType & ptrDT = static_cast<const PtrDataType&>(arrayExprValDT);
        arrayElemDT = &ptrDT.mPointedToType;
    }
    else {
        // Note: no error in the case of an 'undefined' type since this means an error was already emitted elsewhere.
        if (!arrayExprValDT.isUndefined()) {
            // TODO: this message will need to be updated once we support indexing on user types
            mCtx.error("Can't perform array indexing on an expression of type '%s'! "
                       "Currently only arrays and pointer data types can be indexed.",
                       arrayExprValDT.name().c_str());
        }
        
        exprTypeAndIndexTypesAreOk = false;
    }
    
    // Index expression must be an integer, perform that check here:
    const CompiledDataType & indexCDT = indexVal.mCompiledType;
    const DataType & indexDT = indexCDT.getDataType();
    
    if (!indexDT.isInteger()) {
        // Note: no error in the case of an 'undefined' type since this means an error was already emitted elsewhere.
        if (!indexDT.isUndefined()) {
            mCtx.error("Index expression for array lookup must be an integer not type '%s'! "
                       "Can't index an array with non-integer types!",
                       indexDT.name().c_str());
        }

        exprTypeAndIndexTypesAreOk = false;
    }
    
    // Proceed no further if any of these checks fail
    WC_GUARD(indexVal.isValid() &&
             arrayExprVal.isValid() &&
             exprTypeAndIndexTypesAreOk);
    
    // Get the address for array element being indexed:
    llvm::Value * arrayElemAddr = nullptr;
    
    if (arrayExprValDT.isArray()) {
        // Array indexing an array, use the array style GEP instruction:
        llvm::ConstantInt * zeroIndex = llvm::ConstantInt::get(llvm::Type::getInt64Ty(mCtx.mLLVMCtx), 0);
        WC_ASSERT(zeroIndex);
        arrayElemAddr = mCtx.mIRBuilder.CreateGEP(arrayExprVal.mLLVMVal,
                                                  { zeroIndex, indexVal.mLLVMVal },
                                                  "AddrCodegen:PostfixExprArrayLookup:ElemAddr");
    }
    else {
        // Array indexing a pointer.
        // Note: the pointer may need a load if we got it through a variable (l-value), so that first:
        llvm::Value * ptrVal = arrayExprVal.mRequiresLoad ?
            mCtx.mIRBuilder.CreateLoad(arrayExprVal.mLLVMVal, "AddrCodegen:PostfixExprArrayLookup:LoadPtrVal") :
            arrayExprVal.mLLVMVal;
        
        // Now do a pointer arithmetic style GEP instruction:
        arrayElemAddr = mCtx.mIRBuilder.CreateGEP(ptrVal,
                                                  indexVal.mLLVMVal,
                                                  "AddrCodegen:PostfixExprArrayLookup:ElemAddr");
    }
    
    WC_ASSERT(arrayElemAddr);
    
    // Figure out the compiled data type for the element type:
    WC_ASSERT(arrayElemDT);
    arrayElemDT->accept(mCodegenDataType);
    CompiledDataType arrayElemCDT = mCtx.popCompiledDataType();
    WC_GUARD(arrayElemCDT.isValid());
    
    // All good, save the result!
    mCtx.pushValue(Value(arrayElemAddr, arrayElemCDT, true, &astNode));
}

void AddrCodegen::visit(const AST::PostfixExprFuncCall & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();

    #warning TODO: Only allow taking the address of a type which requires storage
    // Evaluate the expression first:
    astNode.accept(mCodegen);
    Value exprVal = mCtx.popValue();
    
    // The value must be valid for this to work
    WC_GUARD(exprVal.isValid());
    
    // The data type for the return value must be sized
    const CompiledDataType & exprValCDT = exprVal.mCompiledType;
    const DataType & exprValDT = exprValCDT.getDataType();
    
    if (!exprValDT.isSized()) {
        // Note: no error in the case of an 'undefined' type since this means an error was already emitted elsewhere.
        if (!exprValDT.isUndefined()) {
            mCtx.error(astNode,
                       "Can't take the address of a function call result which returns "
                       "unsized type '%s'!",
                       exprValDT.name().c_str());
        }
        
        return;
    }
    
    // Create an alloca to hold the result of the function call and store the result there.
    llvm::Value * llvmStackVal = mCtx.mIRBuilder.CreateAlloca(exprValCDT.getLLVMType(),
                                                              nullptr,
                                                              "AddrCodegen:PostfixExprFuncCall:Alloca");
    
    WC_ASSERT(llvmStackVal);
    WC_ASSERTED_OP(mCtx.mIRBuilder.CreateStore(exprVal.mLLVMVal, llvmStackVal));
    
    // Push it onto the codegen context stack to save the result.
    mCtx.pushValue(Value(llvmStackVal, exprValCDT, true, &astNode));
}

void AddrCodegen::visit(const AST::PrefixExprPtrDeref & astNode) {
    WC_CODEGEN_RECORD_VISITED_NODE();
    constexpr const bool kLoadExprResult = false;
    CodegenPtrDerefUnaryOp(mCodegen, astNode.mExpr, kLoadExprResult).codegen();
}

void AddrCodegen::codegenNotSupportedForNodeTypeError(const AST::ASTNode & node,
                                                      const char * nodeClassName)
{
    mCtx.error(node,
               "Code generating the address of AST node type '%s' is not supported!",
               nodeClassName);
}

void AddrCodegen::cantTakeAddressOfUnaryOpError(const AST::ASTNode & exprNode,
                                                const char * opSymbol,
                                                const char * opName)
{
    // Get the node as an expression
    const AST::IExpr * expr = dynamic_cast<const AST::IExpr*>(&exprNode);
    WC_ASSERT(expr);
    
    // Get the parent of the node (the unary operator node)
    const AST::ASTNode * parent = exprNode.mParent;
    WC_ASSERT(parent);
    
    // Compile the expression to figure out what type it is
    exprNode.accept(mCodegen);
    Value val = mCtx.popValue();
    
    // Issue the error
    mCtx.error(*parent,
               "Can't take the address of the result of unary operator '%s' (%s) for an "
               "expression of type '%s'!",
               opSymbol,
               opName,
               val.mCompiledType.getDataType().name().c_str());
}

void AddrCodegen::cantTakeAddressOfBinaryOpError(const AST::ASTNode & leftExprNode,
                                                 const AST::ASTNode & rightExprNode,
                                                 const char * opSymbol,
                                                 const char * opName)
{
    // Get the left and right nodes as expressions
    const AST::IExpr * leftExpr = dynamic_cast<const AST::IExpr*>(&leftExprNode);
    const AST::IExpr * rightExpr = dynamic_cast<const AST::IExpr*>(&rightExprNode);
    WC_ASSERT(leftExpr);
    WC_ASSERT(rightExpr);
    
    // Get the parent of the left and right expression (the binary operator node)
    const AST::ASTNode * parent = leftExprNode.mParent;
    WC_ASSERT(parent);
    
    // Compile the left and right expressions to figure out what types they are
    leftExprNode.accept(mCodegen);
    Value leftVal = mCtx.popValue();
    rightExprNode.accept(mCodegen);
    Value rightVal = mCtx.popValue();
    
    // Issue the error
    mCtx.error(*parent,
               "Can't take the address of the result of binary operator '%s' (%s) for a "
               "left expression of type '%s' and right expression of type '%s'!",
               opSymbol,
               opName,
               leftVal.mCompiledType.getDataType().name().c_str(),
               rightVal.mCompiledType.getDataType().name().c_str());
}

WC_LLVM_BACKEND_END_NAMESPACE
WC_END_NAMESPACE
