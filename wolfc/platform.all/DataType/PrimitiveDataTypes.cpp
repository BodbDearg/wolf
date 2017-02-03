#include "PrimitiveDataTypes.hpp"

#include "Lexer/TokenType.hpp"
#include "Primitives/BoolDataType.hpp"
#include "Primitives/Int64DataType.hpp"
#include "Primitives/StrDataType.hpp"
#include "Primitives/UnknownDataType.hpp"
#include "Primitives/VoidDataType.hpp"

WC_BEGIN_NAMESPACE

/* Instances of the types themselves! */
static UnknownDataType  gUnknownDataType;
static VoidDataType     gVoidDataType;
static Int64DataType    gInt64DataType;
static BoolDataType     gBoolDataType;
static StrDataType      gStrDataType;   // TODO: remove, this is a temporary data type

const DataType & PrimitiveDataTypes::getUsingTypeId(DataTypeId type) {
    switch (type) {
        // If 'Unknown' is specifically asked for
        case DataTypeId::kUnknown: return gUnknownDataType;
            
        // The basic data types
        case DataTypeId::kVoid: return gVoidDataType;
        case DataTypeId::kInt64: return gInt64DataType;
        case DataTypeId::kBool: return gBoolDataType;
        case DataTypeId::kStr: return gStrDataType;
            
        // All these types are created or customized by the user, hence non-primitive:
        case DataTypeId::kArray:
        case DataTypeId::kArrayUnevalSize:
        case DataTypeId::kArrayBadSize:
            break;
    }
    
    return gUnknownDataType;
}

const DataType & PrimitiveDataTypes::getUsingLangKeyword(TokenType tokenType) {
    switch (tokenType) {
        case TokenType::kVoid: return gVoidDataType;
        case TokenType::kInt: return getDefaultIntType();
        case TokenType::kInt64: return gInt64DataType;
        case TokenType::kBool: return gBoolDataType;
        case TokenType::kString: return gStrDataType;
            
        default:
            break;
    }
    
    return gUnknownDataType;
}

const UnknownDataType & PrimitiveDataTypes::getUnknownDataType() {
    return gUnknownDataType;
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
    // FIXME: should be unsigned
    return DataTypeId::kInt64;
}

const DataType & PrimitiveDataTypes::getDefaultIntType() {
    return getUsingTypeId(getDefaultIntTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultIntTypeId() {
    // TODO: support 32-bit targets here in future
    return DataTypeId::kInt64;
}

const DataType & PrimitiveDataTypes::getDefaultFloatType() {
    return getUsingTypeId(getDefaultFloatTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultFloatTypeId() {
    // FIXME: implement for float
    return DataTypeId::kUnknown;
}

WC_END_NAMESPACE
