#pragma once

#include "WCASTNodeCodegen.hpp"

WC_BEGIN_NAMESPACE

class UnaryExpr;

/*
PrintExpr
    print ( StringLit )
    print ( BinaryExpr )
*/
class PrintExpr : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static PrintExpr * parse(const Token *& tokenPtr);
};

/* print ( StringLit ) */
class PrintExprStrLit : public ASTNodeCodegen {
public:
    static bool peek(const Token * tokenPtr);
    
    static PrintExpr * parse(const Token *& tokenPtr);
};

WC_END_NAMESPACE
