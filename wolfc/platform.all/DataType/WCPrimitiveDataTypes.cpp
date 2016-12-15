#include "WCPrimitiveDataTypes.hpp"

#include "DataType/Primitives/WCBoolDataType.hpp"
#include "DataType/Primitives/WCIntDataType.hpp"
#include "DataType/Primitives/WCStrDataType.hpp"
#include "DataType/Primitives/WCUnknownDataType.hpp"
#include "DataType/Primitives/WCVoidDataType.hpp"
#include "Lexer/WCToken.hpp"

WC_BEGIN_NAMESPACE

/* Instances of the types themselves! */
static UnknownDataType  gUnknownDataType;
static VoidDataType     gVoidDataType;
static IntDataType      gIntDataType;
static BoolDataType     gBoolDataType;
static StrDataType      gStrDataType;   // TODO: remove, this is a temporary data type

DataType & PrimitiveDataTypes::getUsingTypeId(DataTypeId type) {
    switch (type) {
        // If 'Unknown' is specifically asked for
        case DataTypeId::kUnknown: return gUnknownDataType;
            
        // The basic data types
        case DataTypeId::kVoid: return gVoidDataType;
        case DataTypeId::kInt: return gIntDataType;
        case DataTypeId::kBool: return gBoolDataType;
        case DataTypeId::kStr: return gStrDataType;
            
        // All these types are created or customized by the user, hence non-primitive:
        case DataTypeId::kArray:
            break;
            
        // This is not a valid value!
        case DataTypeId::kNumTypes:
            break;
    }
    
    return gUnknownDataType;
}

DataType & PrimitiveDataTypes::getUsingLangKeyword(TokenType tokenType) {
    switch (tokenType) {
        case TokenType::kVoid: return gVoidDataType;
        case TokenType::kInt: return gIntDataType;
        case TokenType::kBool: return gBoolDataType;
        case TokenType::kString: return gStrDataType;
            
        default:
            break;
    }
    
    return gUnknownDataType;
}

WC_END_NAMESPACE
