#include "PrimitiveDataTypes.hpp"

#include "Lexer/TokenType.hpp"
#include "Primitives/BoolDataType.hpp"
#include "Primitives/Int64DataType.hpp"
#include "Primitives/InvalidDataType.hpp"
#include "Primitives/StrDataType.hpp"
#include "Primitives/VoidDataType.hpp"

WC_BEGIN_NAMESPACE

/* Instances of the types themselves! */
static InvalidDataType  gInvalidDataType;
static VoidDataType     gVoidDataType;
static Int64DataType    gInt64DataType;
static BoolDataType     gBoolDataType;
static StrDataType      gStrDataType;   // TODO: remove, this is a temporary data type

const DataType & PrimitiveDataTypes::getUsingTypeId(DataTypeId type) {
    switch (type) {
        // Non user facing types:
        case DataTypeId::kInvalid: return gInvalidDataType;
            
        // The basic data types
        case DataTypeId::kVoid: return gVoidDataType;
        case DataTypeId::kInt64: return gInt64DataType;
        case DataTypeId::kBool: return gBoolDataType;
        case DataTypeId::kStr: return gStrDataType;
            
        // All these types are customized types, hence non-primitive:
        case DataTypeId::kArray:
        case DataTypeId::kArrayUnevalSize:
        case DataTypeId::kArrayBadSize:
        case DataTypeId::kFunc:
        case DataTypeId::kFuncUneval:
            break;
    }
    
    return gInvalidDataType;
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
    
    return gInvalidDataType;
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
    return DataTypeId::kInvalid;
}

WC_END_NAMESPACE
