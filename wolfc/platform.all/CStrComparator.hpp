#pragma once

#include "Macros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstring>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

struct CStrComparator {
    bool operator()(const char * s1, const char * s2) const {
        return std::strcmp(s1, s2) < 0;
    }
};

WC_END_NAMESPACE
