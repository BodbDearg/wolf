#pragma once

#include "WCMacros.hpp"

/**
 * A series of helpful macros for implementing boilerplate code related
 * to the visitor pattern. Helps ensure consistency and reduces redundancy.
 */

/**
 * Declares that a class interface must support accepting a visitor of the specified type.
 * This macro requires that all derived classes implement the appropriate version of 'accept()'.
 */
#define WC_MUST_ACCEPT_VISTOR(VisitorType)\
    virtual void accept(VisitorType & vistor) = 0

/**
 * Declares that a class interface will definitely implement accepting a 
 * visitor of the specified type. Used in class definitions to declare that a class implements
 * the 'accept()' function for a specific visitor type.
 */
#define WC_WILL_ACCEPT_VISITOR(VisitorType)\
    virtual void accept(VisitorType & vistor) override

/**
 * Generates the boilerplate code for an implementation of 'accept()'.
 * The first parameter is the class type of the visited object, the second parameter is
 * the type of the visitor.
 */
#define WC_IMPL_ACCEPT_VISITOR(VisitedType, VisitorType)\
    void VisitedType::accept(VisitorType & visitor) {\
        visitor.visit(*this);\
    }

/**
 * Declare a visitor type interface.
 * The first parameter is the visitor type, the 2nd parameter is a list of class types 
 * visited by this interface.
 *
 * Note: there is a limited number of class types this macro can support but if required
 * the limit can be raised.
 */
#define WC_DECLARE_VISITOR_INTERFACE(...)\
    WC_CALL_MACRO_OVERLOAD(__WC_DECLARE_VISITOR_INTERFACE_IMPL_, __VA_ARGS__)

/* Implementations of WC_DECLARE_VISITOR_INTERFACE */
#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    class VistorType {\
    public:\
        virtual ~VistorType() = default;\

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType)\
    virtual void visit(VisitedType & obj) = 0;

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()\
    }

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_1(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_2(VistorType,\
                                              VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_3(VistorType,\
                                              VisitedType1,\
                                              VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_4(VistorType,\
                                              VisitedType1,\
                                              VisitedType2,\
                                              VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_5(VistorType,\
                                              VisitedType1,\
                                              VisitedType2,\
                                              VisitedType3,\
                                              VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_6(VistorType,\
                                              VisitedType1,\
                                              VisitedType2,\
                                              VisitedType3,\
                                              VisitedType4,\
                                              VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_7(VistorType,\
                                              VisitedType1,\
                                              VisitedType2,\
                                              VisitedType3,\
                                              VisitedType4,\
                                              VisitedType5,\
                                              VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_8(VistorType,\
                                              VisitedType1,\
                                              VisitedType2,\
                                              VisitedType3,\
                                              VisitedType4,\
                                              VisitedType5,\
                                              VisitedType6,\
                                              VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_9(VistorType,\
                                              VisitedType1,\
                                              VisitedType2,\
                                              VisitedType3,\
                                              VisitedType4,\
                                              VisitedType5,\
                                              VisitedType6,\
                                              VisitedType7,\
                                              VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_10(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_11(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_12(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_13(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_14(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_15(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13,\
                                               VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_16(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13,\
                                               VisitedType14,\
                                               VisitedType15)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType15)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_17(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13,\
                                               VisitedType14,\
                                               VisitedType15,\
                                               VisitedType16)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType15)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType16)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_18(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13,\
                                               VisitedType14,\
                                               VisitedType15,\
                                               VisitedType16,\
                                               VisitedType17)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType15)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType16)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType17)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_19(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13,\
                                               VisitedType14,\
                                               VisitedType15,\
                                               VisitedType16,\
                                               VisitedType17,\
                                               VisitedType18)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType15)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType16)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType17)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType18)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_20(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13,\
                                               VisitedType14,\
                                               VisitedType15,\
                                               VisitedType16,\
                                               VisitedType17,\
                                               VisitedType18,\
                                               VisitedType19)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType15)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType16)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType17)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType18)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType19)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()

#define __WC_DECLARE_VISITOR_INTERFACE_IMPL_21(VistorType,\
                                               VisitedType1,\
                                               VisitedType2,\
                                               VisitedType3,\
                                               VisitedType4,\
                                               VisitedType5,\
                                               VisitedType6,\
                                               VisitedType7,\
                                               VisitedType8,\
                                               VisitedType9,\
                                               VisitedType10,\
                                               VisitedType11,\
                                               VisitedType12,\
                                               VisitedType13,\
                                               VisitedType14,\
                                               VisitedType15,\
                                               VisitedType16,\
                                               VisitedType17,\
                                               VisitedType18,\
                                               VisitedType19,\
                                               VisitedType20)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_BEGIN(VistorType)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType1)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType2)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType3)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType4)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType5)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType6)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType7)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType8)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType9)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType10)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType11)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType12)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType13)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType14)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType15)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType16)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType17)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType18)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType19)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_DECL_VISIT_TYPE(VisitedType20)\
    __WC_DECLARE_VISITOR_INTERFACE_IMPL_END()
