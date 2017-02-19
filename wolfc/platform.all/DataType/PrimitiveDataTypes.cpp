#include "PrimitiveDataTypes.hpp"

#include "Lexer/TokenType.hpp"
#include "Primitives/BoolDataType.hpp"
#include "Primitives/IntDataTypes.hpp"
#include "Primitives/InvalidDataType.hpp"
#include "Primitives/StrDataType.hpp"
#include "Primitives/VoidDataType.hpp"

WC_BEGIN_NAMESPACE

/* Instances of the types themselves! */
static InvalidDataType  gInvalidDataType;
static VoidDataType     gVoidDataType;
static Int128DataType   gInt128DataType;
static Int16DataType    gInt16DataType;
static Int32DataType    gInt32DataType;
static Int64DataType    gInt64DataType;
static Int8DataType     gInt8DataType;
static UInt128DataType  gUInt128DataType;
static UInt16DataType   gUInt16DataType;
static UInt32DataType   gUInt32DataType;
static UInt64DataType   gUInt64DataType;
static UInt8DataType    gUInt8DataType;
static BoolDataType     gBoolDataType;
static StrDataType      gStrDataType;           // TODO: remove, this is a temporary data type

const DataType & PrimitiveDataTypes::getUsingTypeId(DataTypeId type) {
    switch (type) {
        // Non user facing types:
        case DataTypeId::kInvalid: return gInvalidDataType;
            
        // The basic data types
        case DataTypeId::kVoid: return gVoidDataType;
        case DataTypeId::kInt8: return gInt8DataType;
        case DataTypeId::kInt16: return gInt16DataType;
        case DataTypeId::kInt32: return gInt32DataType;
        case DataTypeId::kInt64: return gInt64DataType;
        case DataTypeId::kInt128: return gInt128DataType;
        case DataTypeId::kUInt8: return gUInt8DataType;
        case DataTypeId::kUInt16: return gUInt16DataType;
        case DataTypeId::kUInt32: return gUInt32DataType;
        case DataTypeId::kUInt64: return gUInt64DataType;
        case DataTypeId::kUInt128: return gUInt128DataType;
        case DataTypeId::kBool: return gBoolDataType;
        case DataTypeId::kStr: return gStrDataType;
            
        // All these types are customized types, hence non-primitive:
        case DataTypeId::kArray:
        case DataTypeId::kFunc:
            break;
    }
    
    return gInvalidDataType;
}

DataTypeId PrimitiveDataTypes::getTypeIdForLangKeyword(TokenType tokenType) {
    switch (tokenType) {
        case TokenType::kVoid: return DataTypeId::kVoid;
        case TokenType::kInt: return getDefaultIntTypeId();
        case TokenType::kInt8: return DataTypeId::kInt8;
        case TokenType::kInt16: return DataTypeId::kInt16;
        case TokenType::kInt32: return DataTypeId::kInt32;
        case TokenType::kInt64: return DataTypeId::kInt64;
        case TokenType::kInt128: return DataTypeId::kInt128;
        case TokenType::kUInt: return getDefaultUIntTypeId();
        case TokenType::kUInt8: return DataTypeId::kUInt8;
        case TokenType::kUInt16: return DataTypeId::kUInt16;
        case TokenType::kUInt32: return DataTypeId::kUInt32;
        case TokenType::kUInt64: return DataTypeId::kUInt64;
        case TokenType::kUInt128: return DataTypeId::kUInt128;
        case TokenType::kBool: return DataTypeId::kBool;
        case TokenType::kString: return DataTypeId::kStr;
            
        default:
            break;
    }
    
    return DataTypeId::kInvalid;
}

const DataType & PrimitiveDataTypes::getUsingLangKeyword(TokenType tokenType) {
    return getUsingTypeId(getTypeIdForLangKeyword(tokenType));
}

bool PrimitiveDataTypes::isLangKeywordPrimitiveType(TokenType tokenType) {
    return getTypeIdForLangKeyword(tokenType) != DataTypeId::kInvalid;
}

const InvalidDataType & PrimitiveDataTypes::getInvalidDataType() {
    return gInvalidDataType;
}

const VoidDataType & PrimitiveDataTypes::getVoidDataType() {
    return gVoidDataType;
}

const BoolDataType & PrimitiveDataTypes::getBoolDataType() {
    return gBoolDataType;
}

const DataType & PrimitiveDataTypes::getDefaultUIntType() {
    return getUsingTypeId(getDefaultUIntTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultUIntTypeId() {
    // TODO: should default to 32-bit for 32-bit targets
    return DataTypeId::kUInt64;
}

const DataType & PrimitiveDataTypes::getDefaultIntType() {
    return getUsingTypeId(getDefaultIntTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultIntTypeId() {
    // TODO: should default to 32-bit for 32-bit targets
    return DataTypeId::kInt64;
}

const DataType & PrimitiveDataTypes::getDefaultFloatType() {
    return getUsingTypeId(getDefaultFloatTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultFloatTypeId() {
    // FIXME: implement for float
    return DataTypeId::kInvalid;
}

WC_END_NAMESPACE
