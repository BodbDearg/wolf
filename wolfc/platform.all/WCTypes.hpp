#pragma once

#include <cstdint>
#include <string>

/**
 * Some compiler primitives.
 */

/* The character type used by the compiler: UTF32 */
typedef uint32_t uchar_t;

/* String type used by the compiler: string of UTF32 chars */
typedef std::basic_string<uint32_t> ustring_t;
