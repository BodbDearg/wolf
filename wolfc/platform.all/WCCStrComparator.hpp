#pragma once

#include "WCMacros.hpp"
#include "WCStringUtils.hpp"

WC_BEGIN_NAMESPACE

struct CStrComparator {
    bool operator()(const char32_t * s1, const char32_t * s2) const {
        return StringUtils::strcmp(s1, s2) < static_cast<int32_t>(0);
    }
};

WC_END_NAMESPACE
