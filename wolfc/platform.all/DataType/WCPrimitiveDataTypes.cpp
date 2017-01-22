#include "WCPrimitiveDataTypes.hpp"

#include "DataType/Primitives/WCBoolDataType.hpp"
#include "DataType/Primitives/WCInt64DataType.hpp"
#include "DataType/Primitives/WCStrDataType.hpp"
#include "DataType/Primitives/WCUnknownDataType.hpp"
#include "DataType/Primitives/WCVoidDataType.hpp"
#include "Lexer/WCTokenType.hpp"

WC_BEGIN_NAMESPACE

/* Instances of the types themselves! */
static UnknownDataType  gUnknownDataType;
static VoidDataType     gVoidDataType;
static Int64DataType    gInt64DataType;
static BoolDataType     gBoolDataType;
static StrDataType      gStrDataType;   // TODO: remove, this is a temporary data type

DataType & PrimitiveDataTypes::getUsingTypeId(DataTypeId type) {
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
            break;
    }
    
    return gUnknownDataType;
}

DataType & PrimitiveDataTypes::getUsingLangKeyword(TokenType tokenType) {
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

DataType & PrimitiveDataTypes::getDefaultUIntType() {
    return getUsingTypeId(getDefaultUIntTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultUIntTypeId() {
    // FIXME: should be unsigned
    return DataTypeId::kInt64;
}

DataType & PrimitiveDataTypes::getDefaultIntType() {
    return getUsingTypeId(getDefaultIntTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultIntTypeId() {
    // TODO: support 32-bit targets here in future
    return DataTypeId::kInt64;
}

DataType & PrimitiveDataTypes::getDefaultFloatType() {
    return getUsingTypeId(getDefaultFloatTypeId());
}

DataTypeId PrimitiveDataTypes::getDefaultFloatTypeId() {
    // FIXME: implement for float
    return DataTypeId::kUnknown;
}

WC_END_NAMESPACE
