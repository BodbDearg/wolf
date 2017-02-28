//==============================================================================
// The Wolf Programming Language.
//      Copyright (c) 2016, Darragh Coy.
//      See LICENSE.txt for license details.
//==============================================================================

#pragma once

#include "DataTypeId.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <string>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

namespace AST {
    class ASTNode;
}

class DataTypeVisitor;

/* Base class for a data type */
class DataType {
public:
    virtual ~DataType();
    
    /* Visitor pattern accept function */
    virtual void accept(DataTypeVisitor & visitor) const = 0;
    
    /* Get the id of the data type */
    virtual DataTypeId getTypeId() const = 0;
    
    /* Give the name of the datatype, e.g 'int32' */
    virtual const std::string & name() const = 0;
    
    /* Tells if this data type matches another. */
    virtual bool equals(const DataType & other) const = 0;
    
    /**
     * Tells if the data type is valid. 
     * Returns 'true' for all data types except unknown data types.
     */
    virtual bool isValid() const;
    
    /**
     * Utility. Tell if the data type occupies any space (size concept makes sense).
     * Most data types are sized, with the exception of the unknown data type and void.
     */
    virtual bool isSized() const;
    
    /* Utility. Tell if this data type is the 'void' data type. */
    inline bool isVoid() const {
        return getTypeId() == DataTypeId::kVoid;
    }
    
    /* Utility. Tell if this data type is the 'bool' data type. */
    inline bool isBool() const {
        return getTypeId() == DataTypeId::kBool;
    }
    
    /* Utility. Tell if this data type is an integer data type. */
    virtual bool isInteger() const;
    
    /** 
     * Utility. Tell if this is a signed data type. Note: for data types where this question 
     * does not make sense the function will simply return 'false'.
     */
    virtual bool isSigned() const;

    /* Utility. Tell if this data type is a pointer data type. */
    virtual bool isPtr() const;
    
    /* Utility. Tell if this data type is an array data type. */
    virtual bool isArray() const;
    
    /* Utility. Tell if this data type is a function data type */
    virtual bool isFunc() const;
    
    /**
     * If true then the data type requires a storage area to hold it's data since it is
     * not a simple primitive that can be held in a register. This will be true for arrays, 
     * structs and other aggregate types.
     */
    virtual bool requiresStorage() const;
};

WC_END_NAMESPACE
