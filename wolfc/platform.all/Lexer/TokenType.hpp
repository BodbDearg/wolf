#pragma once

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdint>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/* Enum representing the type of token */
enum class TokenType : uint8_t {
    /*=========================================================================
    | Special / other
    =========================================================================*/
    
    /* End of file token. Always the last token in a sequence */
    kEOF,
    /* New line token */
    kNewline,
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
    /* '++' */
    kIncrement,
    /* '--' */
    kDecrement,
    /* % */
    kPercent,
    /* '*' */
    kAsterisk,
    /* '/' */
    kSlash,
    /* '=' */
    kAssign,
    /* '<<=' */
    kAssignLShift,
    /* '>>=' */
    kAssignARShift,
    /* '>>>=' */
    kAssignLRShift,
    /* '|=' */
    kAssignBOr,
    /* '^=' */
    kAssignBXor,
    /* '&=' */
    kAssignBAnd,
    /* '+=' */
    kAssignAdd,
    /* '-=' */
    kAssignSub,
    /* '*=' */
    kAssignMul,
    /* '/=' */
    kAssignDiv,
    /* '%=' */
    kAssignMod,
    /* '==' */
    kCmpEQ,
    /* '!=' */
    kCmpNE,
    /* '<' */
    kCmpLT,
    /* '<=' */
    kCmpLE,
    /* '>' */
    kCmpGT,
    /* '>=' */
    kCmpGE,
    /* '<<' */
    kLShift,
    /* '>>' */
    kARShift,
    /* '>>>' */
    kLRShift,
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
    /* ~ */
    kTilde,
    /* & */
    kAmpersand,
    /* $ */
    kDollar,
    /* ^ */
    kHat,
    /* | */
    kVBar,

    /*=========================================================================
    | Language keywords: data types
    =========================================================================*/
    
    /* 'void' */
    kVoid,
    /* 'int' */
    kInt,
    /* 'int8' */
    kInt8,
    /* 'int16' */
    kInt16,
    /* 'int32' */
    kInt32,
    /* 'int64' */
    kInt64,
    /* 'int128' */
    kInt128,
    /* 'uint' */
    kUInt,
    /* 'uint8' */
    kUInt8,
    /* 'uint16' */
    kUInt16,
    /* 'uint32' */
    kUInt32,
    /* 'uint64' */
    kUInt64,
    /* 'uint128' */
    kUInt128,
    /* 'bool' */
    kBool,
    /* 'string' */  // TODO: this is a temporary data type, will eventually be a built-in class
    kString,
    
    /*=========================================================================
    | Language keywords: general
    =========================================================================*/
    
    /* let */
    kLet,
    /* is */
    kIs,
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
    /* noop */
    kNoOp,
    /* return */
    kReturn,
    /* func */
    kFunc,
    /* cast */
    kCast,
    /* to */
    kTo,
    /* 'time' */   // TODO: this is a temporary keyword, remove eventually
    kTime,
    /* 'rand' */   // TODO: this is a temporary keyword, remove eventually
    kRand,
    /* 'srand' */   // TODO: this is a temporary keyword, remove eventually
    kSRand,
    /* 'print' */   // TODO: this is a temporary keyword, remove eventually
    kPrint,
    /* 'readnum' */ // TODO: this is a temporary keyword, remove eventually
    kReadnum,
    /* 'assert' */
    kAssert,
};

WC_END_NAMESPACE
