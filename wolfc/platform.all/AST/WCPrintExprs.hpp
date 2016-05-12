#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class PrintExpr;

/*
PrintExprs
    PrintExpr
    PrintExpr PrintExprs
*/
class PrintExprs : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static PrintExprs * parse(const Token *& tokenPtr);
};

/* PrintExpr */
class PrintExprsSingle : public PrintExprs {
public:
    PrintExprsSingle(PrintExpr & expr);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrintExpr & mExpr;
};

/* PrintExpr PrintExprs */
class PrintExprsMulti : public PrintExprs {
public:
    PrintExprsMulti(PrintExpr & startExpr, PrintExprs & endExprs);
    
    virtual llvm::Value * generateCode(const CodegenCtx & cgCtx) override;
    
    PrintExpr & mStartExpr;
    PrintExprs & mEndExprs;
};

WC_END_NAMESPACE
