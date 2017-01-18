#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

/* Interface for a visitor to AST nodes */
class ASTNodeVisitor {
public:
    virtual ~ASTNodeVisitor() = default;
    
    virtual void visit(class AddExprAdd & node) = 0;
    virtual void visit(class AddExprBOr & node) = 0;
    virtual void visit(class AddExprBXor & node) = 0;
    virtual void visit(class AddExprNoOp & node) = 0;
    virtual void visit(class AddExprSub & node) = 0;
    virtual void visit(class AddExprTwoOps & node) = 0;
    virtual void visit(class ArrayLit & node) = 0;
    virtual void visit(class ArrayLitExprs & node) = 0;
    virtual void visit(class ArrayLitExprsMulti & node) = 0;
    virtual void visit(class ArrayLitExprsSingle & node) = 0;
    virtual void visit(class AssertStmnt & node) = 0;
    virtual void visit(class AssignExprAssign & node) = 0;
    virtual void visit(class AssignExprAssignAdd & node) = 0;
    virtual void visit(class AssignExprAssignARShift & node) = 0;
    virtual void visit(class AssignExprAssignBAnd & node) = 0;
    virtual void visit(class AssignExprAssignBase & node) = 0;
    virtual void visit(class AssignExprAssignBOr & node) = 0;
    virtual void visit(class AssignExprAssignBXor & node) = 0;
    virtual void visit(class AssignExprAssignDiv & node) = 0;
    virtual void visit(class AssignExprAssignLRShift & node) = 0;
    virtual void visit(class AssignExprAssignLShift & node) = 0;
    virtual void visit(class AssignExprAssignMod & node) = 0;
    virtual void visit(class AssignExprAssignMul & node) = 0;
    virtual void visit(class AssignExprAssignSub & node) = 0;
    virtual void visit(class AssignExprBinaryOpBase & node) = 0;
    virtual void visit(class AssignExprNoAssign & node) = 0;
    virtual void visit(class BoolLit & node) = 0;
    virtual void visit(class BreakStmntNoCond & node) = 0;
    virtual void visit(class BreakStmntWithCond & node) = 0;
    virtual void visit(class CastExprCast & node) = 0;
    virtual void visit(class CastExprNoCast & node) = 0;
    virtual void visit(class CmpExprEQ & node) = 0;
    virtual void visit(class CmpExprGE & node) = 0;
    virtual void visit(class CmpExprGT & node) = 0;
    virtual void visit(class CmpExprLE & node) = 0;
    virtual void visit(class CmpExprLT & node) = 0;
    virtual void visit(class CmpExprNE & node) = 0;
    virtual void visit(class CmpExprNoOp & node) = 0;
    virtual void visit(class CmpExprTwoOps & node) = 0;
    virtual void visit(class DataType & node) = 0;
    virtual void visit(class DeclDefFunc & node) = 0;
    virtual void visit(class DeclDefVarDecl & node) = 0;
    virtual void visit(class Func & node) = 0;
    virtual void visit(class FuncArg & node) = 0;
    virtual void visit(class FuncArgListMulti & node) = 0;
    virtual void visit(class FuncArgListSingle & node) = 0;
    virtual void visit(class FuncCall & node) = 0;
    virtual void visit(class FuncCallArgListMulti & node) = 0;
    virtual void visit(class FuncCallArgListSingle & node) = 0;
    virtual void visit(class Identifier & node) = 0;
    virtual void visit(class IfStmntElse & node) = 0;
    virtual void visit(class IfStmntElseIf & node) = 0;
    virtual void visit(class IfStmntNoElse & node) = 0;
    virtual void visit(class IntLit & node) = 0;
    virtual void visit(class LAndExprAnd & node) = 0;
    virtual void visit(class LAndExprNoOp & node) = 0;
    virtual void visit(class LoopStmntNoCond & node) = 0;
    virtual void visit(class LoopStmntWithCond & node) = 0;
    virtual void visit(class LOrExprNoOp & node) = 0;
    virtual void visit(class LOrExprOr & node) = 0;
    virtual void visit(class Module & node) = 0;
    virtual void visit(class MulExprBAnd & node) = 0;
    virtual void visit(class MulExprDiv & node) = 0;
    virtual void visit(class MulExprMod & node) = 0;
    virtual void visit(class MulExprMul & node) = 0;
    virtual void visit(class MulExprNoOp & node) = 0;
    virtual void visit(class MulExprTwoOps & node) = 0;
    virtual void visit(class NextStmntNoCond & node) = 0;
    virtual void visit(class NextStmntWithCond & node) = 0;
    virtual void visit(class NoOpStmnt & node) = 0;
    virtual void visit(class NotExprBNot & node) = 0;
    virtual void visit(class NotExprLNot & node) = 0;
    virtual void visit(class NotExprNoOp & node) = 0;
    virtual void visit(class PostfixExprArrayLookup & node) = 0;
    virtual void visit(class PostfixExprDec & node) = 0;
    virtual void visit(class PostfixExprFuncCall & node) = 0;
    virtual void visit(class PostfixExprInc & node) = 0;
    virtual void visit(class PostfixExprIncDecBase & node) = 0;
    virtual void visit(class PostfixExprNoPostfix & node) = 0;
    virtual void visit(class PrimaryExprArrayLit & node) = 0;
    virtual void visit(class PrimaryExprBoolLit & node) = 0;
    virtual void visit(class PrimaryExprIdentifier & node) = 0;
    virtual void visit(class PrimaryExprIntLit & node) = 0;
    virtual void visit(class PrimaryExprRandExpr & node) = 0;
    virtual void visit(class PrimaryExprReadnum & node) = 0;
    virtual void visit(class PrimaryExprStrLit & node) = 0;
    virtual void visit(class PrimaryExprTime & node) = 0;
    virtual void visit(class PrimitiveType & node) = 0;
    virtual void visit(class PrintStmnt & node) = 0;
    virtual void visit(class RandExprRand & node) = 0;
    virtual void visit(class RandExprSRand & node) = 0;
    virtual void visit(class ReadnumExpr & node) = 0;
    virtual void visit(class ReturnStmntNoCondVoid & node) = 0;
    virtual void visit(class ReturnStmntNoCondWithValue & node) = 0;
    virtual void visit(class ReturnStmntWithCondAndValue & node) = 0;
    virtual void visit(class ReturnStmntWithCondBase & node) = 0;
    virtual void visit(class ReturnStmntWithCondVoid & node) = 0;
    virtual void visit(class Scope & node) = 0;
    virtual void visit(class ScopeStmnt & node) = 0;
    virtual void visit(class ShiftExprARShift & node) = 0;
    virtual void visit(class ShiftExprLRShift & node) = 0;
    virtual void visit(class ShiftExprLShift & node) = 0;
    virtual void visit(class ShiftExprNoOp & node) = 0;
    virtual void visit(class ShiftExprTwoOps & node) = 0;
    virtual void visit(class StmntAssertStmnt & node) = 0;
    virtual void visit(class StmntAssignExpr & node) = 0;
    virtual void visit(class StmntBreakStmnt & node) = 0;
    virtual void visit(class StmntIfStmnt & node) = 0;
    virtual void visit(class StmntLoopStmnt & node) = 0;
    virtual void visit(class StmntNextStmnt & node) = 0;
    virtual void visit(class StmntNoOpStmnt & node) = 0;
    virtual void visit(class StmntPrintStmnt & node) = 0;
    virtual void visit(class StmntReturnStmnt & node) = 0;
    virtual void visit(class StmntScopeStmnt & node) = 0;
    virtual void visit(class StmntVarDecl & node) = 0;
    virtual void visit(class StmntWhileStmnt & node) = 0;
    virtual void visit(class StrLit & node) = 0;
    virtual void visit(class TernaryExprNoCond & node) = 0;
    virtual void visit(class TernaryExprWithCond & node) = 0;
    virtual void visit(class TimeExpr & node) = 0;
    virtual void visit(class TypeArray & node) = 0;
    virtual void visit(class TypePrimitive & node) = 0;
    virtual void visit(class UnaryExprMinus & node) = 0;
    virtual void visit(class UnaryExprParen & node) = 0;
    virtual void visit(class UnaryExprPlus & node) = 0;
    virtual void visit(class UnaryExprPlusMinusBase & node) = 0;
    virtual void visit(class UnaryExprPrimary & node) = 0;
    virtual void visit(class VarDeclExplicitType & node) = 0;
    virtual void visit(class VarDeclInferType & node) = 0;
    virtual void visit(class WhileStmnt & node) = 0;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
