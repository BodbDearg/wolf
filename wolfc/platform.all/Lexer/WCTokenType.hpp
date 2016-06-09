#pragma once

/* Enum representing the type of token */
enum class TokenType {
    /* End of file token. Always the last token in a sequence */
    kEOF,
    /* Unsigned integer literal, like '01203'. The value is stored in the 'data' field as 'intVal'. */
    kIntLit,
    /* String literal, like "Hello World". The value is store in the 'data' field as 'strVal'. */
    kStrLit,
    /**
     * A name/idenifier component for variables, function and class names etc.
     * The identifeir name is stored in the 'data' field as 'strVal'.
     */
    kIdentifier,
    /* '(' */
    kLParen,
    /* ')' */
    kRParen,
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
    /* var */
    kVar,
    /* true */
    kTrue,
    /* false */
    kFalse,
    /* and */
    kAnd,
    /* or */
    kOr,
    /* not */
    kNot,
    /* if */
    kIf,
    /* then */
    kThen,
    /* else */
    kElse,
    /* end */
    kEnd,
    /* nop */
    kNop,
    /* TEMPORARY: print */
    kPrint,
    /* TEMPORARY: readnum */
    kReadnum,
};
