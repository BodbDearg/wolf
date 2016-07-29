#pragma once

/* Enum representing the type of token */
enum class TokenType {
    /*=========================================================================
    | Special / other
    =========================================================================*/
    
    /* End of file token. Always the last token in a sequence */
    kEOF,
    /**
     * A name/idenifier component for variables, function and class names etc.
     * The identifeir name is stored in the 'data' field as 'strVal'.
     */
    kIdentifier,
    
    /*=========================================================================
    | Literals
    =========================================================================*/
    
    /* Unsigned integer literal, like '01203'. The value is stored in the 'data' field as 'intVal'. */
    kIntLit,
    /* String literal, like "Hello World". The value is store in the 'data' field as 'strVal'. */
    kStrLit,
    /* 'true' keyword/bool-literal */
    kTrue,
    /* 'false' keyword/bool-literal */
    kFalse,
    
    /*=========================================================================
    | Tokens and operators
    =========================================================================*/
    
    /* '(' */
    kLParen,
    /* ')' */
    kRParen,
    /* '[' */
    kLBrack,
    /* ']' */
    kRBrack,
    /* '+' */
    kPlus,
    /* '-' */
    kMinus,
    /* '*' */
    kAsterisk,
    /* '/' */
    kSlash,
    /* '<' */
    kLessThan,
    /* '>' */
    kGreaterThan,
    /* '=' */
    kEquals,
    /* '!' */
    kExclamation,
    /* ',' */
    kComma,
    /* -> */
    kOpArrow,
    /* : */
    kColon,
    /* ? */
    kQMark,

    /*=========================================================================
    | Language keywords: data types
    =========================================================================*/
    
    /* 'void' */
    kVoid,
    /* 'int' */
    kInt,
    /* 'bool' */
    kBool,
    /* 'string' */  // TODO: this is a temporary data type, will eventually be a built-in class
    kString,
    
    /*=========================================================================
    | Language keywords: general
    =========================================================================*/
    
    /* var */
    kVar,
    /* and */
    kAnd,
    /* or */
    kOr,
    /* not */
    kNot,
    /* if */
    kIf,
    /* unless */
    kUnless,
    /* then */
    kThen,
    /* else */
    kElse,
    /* end */
    kEnd,
    /* while */
    kWhile,
    /* until */
    kUntil,
    /* do */
    kDo,
    /* break */
    kBreak,
    /* next */
    kNext,
    /* loop */
    kLoop,
    /* repeat */
    kRepeat,
    /* scope */
    kScope,
    /* nop */
    kNop,
    /* return */
    kReturn,
    /* func */
    kFunc,
    /* 'print' */   // TODO: this is a temporary keyword, remove eventually
    kPrint,
    /* 'readnum' */ // TODO: this is a temporary keyword, remove eventually
    kReadnum,
};
