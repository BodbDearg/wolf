#pragma once

#include "WCMacros.hpp"

WC_BEGIN_NAMESPACE
WC_AST_BEGIN_NAMESPACE

class AddExprAdd;
class AddExprBOr;
class AddExprBXor;
class AddExprNoOp;
class AddExprSub;
class ArrayLit;
class ArrayLitExprs;
class ArrayLitExprsMulti;
class ArrayLitExprsSingle;
class AssertStmnt;
class AssignExprAssign;
class AssignExprAssignAdd;
class AssignExprAssignARShift;
class AssignExprAssignBAnd;
class AssignExprAssignBOr;
class AssignExprAssignBXor;
class AssignExprAssignDiv;
class AssignExprAssignLRShift;
class AssignExprAssignLShift;
class AssignExprAssignMod;
class AssignExprAssignMul;
class AssignExprAssignSub;
class AssignExprNoAssign;
class BoolLit;
class BreakStmntNoCond;
class BreakStmntWithCond;
class CastExprCast;
class CastExprNoCast;
class CmpExprEQ;
class CmpExprGE;
class CmpExprGT;
class CmpExprLE;
class CmpExprLT;
class CmpExprNE;
class CmpExprNoOp;
class DeclDefFunc;
class DeclDefVarDecl;
class Func;
class FuncArg;
class FuncCall;
class FuncCallArgListMulti;
class FuncCallArgListSingle;
class Identifier;
class IfStmntElse;
class IfStmntElseIf;
class IfStmntNoElse;
class IntLit;
class LAndExprAnd;
class LAndExprNoOp;
class LoopStmntNoCond;
class LoopStmntWithCond;
class LOrExprNoOp;
class LOrExprOr;
class Module;
class MulExprBAnd;
class MulExprDiv;
class MulExprMod;
class MulExprMul;
class MulExprNoOp;
class NextStmntNoCond;
class NextStmntWithCond;
class NoOpStmnt;
class NotExprBNot;
class NotExprLNot;
class NotExprNoOp;
class PostfixExprArrayLookup;
class PostfixExprDec;
class PostfixExprFuncCall;
class PostfixExprInc;
class PostfixExprNoPostfix;
class PrimaryExprArrayLit;
class PrimaryExprBoolLit;
class PrimaryExprIdentifier;
class PrimaryExprIntLit;
class PrimaryExprRandExpr;
class PrimaryExprReadnum;
class PrimaryExprStrLit;
class PrimaryExprTime;
class PrimitiveType;
class PrintStmnt;
class RandExprRand;
class RandExprSRand;
class ReadnumExpr;
class ReturnStmntNoCondVoid;
class ReturnStmntNoCondWithValue;
class ReturnStmntWithCondAndValue;
class ReturnStmntWithCondVoid;
class Scope;
class ScopeStmnt;
class ShiftExprARShift;
class ShiftExprLRShift;
class ShiftExprLShift;
class ShiftExprNoOp;
class StmntAssertStmnt;
class StmntAssignExpr;
class StmntBreakStmnt;
class StmntIfStmnt;
class StmntLoopStmnt;
class StmntNextStmnt;
class StmntNoOpStmnt;
class StmntPrintStmnt;
class StmntReturnStmnt;
class StmntScopeStmnt;
class StmntVarDecl;
class StmntWhileStmnt;
class StrLit;
class TernaryExprNoCond;
class TernaryExprWithCond;
class TimeExpr;
class TypeArray;
class TypePrimitive;
class UnaryExprMinus;
class UnaryExprParen;
class UnaryExprPlus;
class UnaryExprPrimary;
class VarDeclExplicitType;
class VarDeclInferType;
class WhileStmnt;

/* Interface for a visitor to AST nodes */
class ASTNodeVisitor {
public:
    virtual ~ASTNodeVisitor() = default;
    
    virtual void visit(const AddExprAdd & astNode) = 0;
    virtual void visit(const AddExprBOr & astNode) = 0;
    virtual void visit(const AddExprBXor & astNode) = 0;
    virtual void visit(const AddExprNoOp & astNode) = 0;
    virtual void visit(const AddExprSub & astNode) = 0;
    virtual void visit(const ArrayLit & astNode) = 0;
    virtual void visit(const ArrayLitExprs & astNode) = 0;
    virtual void visit(const ArrayLitExprsMulti & astNode) = 0;
    virtual void visit(const ArrayLitExprsSingle & astNode) = 0;
    virtual void visit(const AssertStmnt & astNode) = 0;
    virtual void visit(const AssignExprAssign & astNode) = 0;
    virtual void visit(const AssignExprAssignAdd & astNode) = 0;
    virtual void visit(const AssignExprAssignARShift & astNode) = 0;
    virtual void visit(const AssignExprAssignBAnd & astNode) = 0;
    virtual void visit(const AssignExprAssignBOr & astNode) = 0;
    virtual void visit(const AssignExprAssignBXor & astNode) = 0;
    virtual void visit(const AssignExprAssignDiv & astNode) = 0;
    virtual void visit(const AssignExprAssignLRShift & astNode) = 0;
    virtual void visit(const AssignExprAssignLShift & astNode) = 0;
    virtual void visit(const AssignExprAssignMod & astNode) = 0;
    virtual void visit(const AssignExprAssignMul & astNode) = 0;
    virtual void visit(const AssignExprAssignSub & astNode) = 0;
    virtual void visit(const AssignExprNoAssign & astNode) = 0;
    virtual void visit(const BoolLit & astNode) = 0;
    virtual void visit(const BreakStmntNoCond & astNode) = 0;
    virtual void visit(const BreakStmntWithCond & astNode) = 0;
    virtual void visit(const CastExprCast & astNode) = 0;
    virtual void visit(const CastExprNoCast & astNode) = 0;
    virtual void visit(const CmpExprEQ & astNode) = 0;
    virtual void visit(const CmpExprGE & astNode) = 0;
    virtual void visit(const CmpExprGT & astNode) = 0;
    virtual void visit(const CmpExprLE & astNode) = 0;
    virtual void visit(const CmpExprLT & astNode) = 0;
    virtual void visit(const CmpExprNE & astNode) = 0;
    virtual void visit(const CmpExprNoOp & astNode) = 0;
    virtual void visit(const DeclDefFunc & astNode) = 0;
    virtual void visit(const DeclDefVarDecl & astNode) = 0;
    virtual void visit(const Func & astNode) = 0;
    virtual void visit(const FuncArg & astNode) = 0;
    virtual void visit(const FuncCall & astNode) = 0;
    virtual void visit(const FuncCallArgListMulti & astNode) = 0;
    virtual void visit(const FuncCallArgListSingle & astNode) = 0;
    virtual void visit(const Identifier & astNode) = 0;
    virtual void visit(const IfStmntElse & astNode) = 0;
    virtual void visit(const IfStmntElseIf & astNode) = 0;
    virtual void visit(const IfStmntNoElse & astNode) = 0;
    virtual void visit(const IntLit & astNode) = 0;
    virtual void visit(const LAndExprAnd & astNode) = 0;
    virtual void visit(const LAndExprNoOp & astNode) = 0;
    virtual void visit(const LoopStmntNoCond & astNode) = 0;
    virtual void visit(const LoopStmntWithCond & astNode) = 0;
    virtual void visit(const LOrExprNoOp & astNode) = 0;
    virtual void visit(const LOrExprOr & astNode) = 0;
    virtual void visit(const Module & astNode) = 0;
    virtual void visit(const MulExprBAnd & astNode) = 0;
    virtual void visit(const MulExprDiv & astNode) = 0;
    virtual void visit(const MulExprMod & astNode) = 0;
    virtual void visit(const MulExprMul & astNode) = 0;
    virtual void visit(const MulExprNoOp & astNode) = 0;
    virtual void visit(const NextStmntNoCond & astNode) = 0;
    virtual void visit(const NextStmntWithCond & astNode) = 0;
    virtual void visit(const NoOpStmnt & astNode) = 0;
    virtual void visit(const NotExprBNot & astNode) = 0;
    virtual void visit(const NotExprLNot & astNode) = 0;
    virtual void visit(const NotExprNoOp & astNode) = 0;
    virtual void visit(const PostfixExprArrayLookup & astNode) = 0;
    virtual void visit(const PostfixExprDec & astNode) = 0;
    virtual void visit(const PostfixExprFuncCall & astNode) = 0;
    virtual void visit(const PostfixExprInc & astNode) = 0;
    virtual void visit(const PostfixExprNoPostfix & astNode) = 0;
    virtual void visit(const PrimaryExprArrayLit & astNode) = 0;
    virtual void visit(const PrimaryExprBoolLit & astNode) = 0;
    virtual void visit(const PrimaryExprIdentifier & astNode) = 0;
    virtual void visit(const PrimaryExprIntLit & astNode) = 0;
    virtual void visit(const PrimaryExprRandExpr & astNode) = 0;
    virtual void visit(const PrimaryExprReadnum & astNode) = 0;
    virtual void visit(const PrimaryExprStrLit & astNode) = 0;
    virtual void visit(const PrimaryExprTime & astNode) = 0;
    virtual void visit(const PrimitiveType & astNode) = 0;
    virtual void visit(const PrintStmnt & astNode) = 0;
    virtual void visit(const RandExprRand & astNode) = 0;
    virtual void visit(const RandExprSRand & astNode) = 0;
    virtual void visit(const ReadnumExpr & astNode) = 0;
    virtual void visit(const ReturnStmntNoCondVoid & astNode) = 0;
    virtual void visit(const ReturnStmntNoCondWithValue & astNode) = 0;
    virtual void visit(const ReturnStmntWithCondAndValue & astNode) = 0;
    virtual void visit(const ReturnStmntWithCondVoid & astNode) = 0;
    virtual void visit(const Scope & astNode) = 0;
    virtual void visit(const ScopeStmnt & astNode) = 0;
    virtual void visit(const ShiftExprARShift & astNode) = 0;
    virtual void visit(const ShiftExprLRShift & astNode) = 0;
    virtual void visit(const ShiftExprLShift & astNode) = 0;
    virtual void visit(const ShiftExprNoOp & astNode) = 0;
    virtual void visit(const StmntAssertStmnt & astNode) = 0;
    virtual void visit(const StmntAssignExpr & astNode) = 0;
    virtual void visit(const StmntBreakStmnt & astNode) = 0;
    virtual void visit(const StmntIfStmnt & astNode) = 0;
    virtual void visit(const StmntLoopStmnt & astNode) = 0;
    virtual void visit(const StmntNextStmnt & astNode) = 0;
    virtual void visit(const StmntNoOpStmnt & astNode) = 0;
    virtual void visit(const StmntPrintStmnt & astNode) = 0;
    virtual void visit(const StmntReturnStmnt & astNode) = 0;
    virtual void visit(const StmntScopeStmnt & astNode) = 0;
    virtual void visit(const StmntVarDecl & astNode) = 0;
    virtual void visit(const StmntWhileStmnt & astNode) = 0;
    virtual void visit(const StrLit & astNode) = 0;
    virtual void visit(const TernaryExprNoCond & astNode) = 0;
    virtual void visit(const TernaryExprWithCond & astNode) = 0;
    virtual void visit(const TimeExpr & astNode) = 0;
    virtual void visit(const TypeArray & astNode) = 0;
    virtual void visit(const TypePrimitive & astNode) = 0;
    virtual void visit(const UnaryExprMinus & astNode) = 0;
    virtual void visit(const UnaryExprParen & astNode) = 0;
    virtual void visit(const UnaryExprPlus & astNode) = 0;
    virtual void visit(const UnaryExprPrimary & astNode) = 0;
    virtual void visit(const VarDeclExplicitType & astNode) = 0;
    virtual void visit(const VarDeclInferType & astNode) = 0;
    virtual void visit(const WhileStmnt & astNode) = 0;
};

WC_AST_END_NAMESPACE
WC_END_NAMESPACE
