#include "WCPrimitiveDataTypes.hpp"
#include "WCBoolDataType.hpp"
#include "WCIntDataType.hpp"
#include "WCStrDataType.hpp"
#include "WCToken.hpp"
#include "WCUnknownDataType.hpp"
#include "WCVoidDataType.hpp"

WC_BEGIN_NAMESPACE

/* Instances of the types themselves! */
static UnknownDataType  gUnknownDataType;
static VoidDataType     gVoidDataType;
static IntDataType      gIntDataType;
static BoolDataType     gBoolDataType;
static StrDataType      gStrDataType;   // TODO: remove, this is a temporary data type

DataType & PrimitiveDataTypes::get(Type type) {
    switch (type) {
        case Type::kUnknown: return gUnknownDataType;
        case Type::kVoid: return gVoidDataType;
        case Type::kInt: return gIntDataType;
        case Type::kBool: return gBoolDataType;
        case Type::kStr: return gStrDataType;
            
        // This is not a valid value!
        case Type::kNumTypes:
            break;
    }
    
    return gUnknownDataType;
}

DataType & PrimitiveDataTypes::dataTypeForLangKeyword(TokenType tokenType) {
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
