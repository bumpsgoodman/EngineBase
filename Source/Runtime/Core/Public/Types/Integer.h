#pragma once

#include <stdint.h>

typedef uint8_t Uint8;
typedef uint16_t Uint16;
typedef uint32_t Uint32;
typedef uint64_t Uint64;

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef Uint8 Byte;

namespace TestIntegerType
{
    static_assert(sizeof(Uint8) == 1, "mismatched size: Uint8");
    static_assert(sizeof(Uint16) == 2, "mismatched size: Uint16");
    static_assert(sizeof(Uint32) == 4, "mismatched size: Uint32");
    static_assert(sizeof(Uint64) == 8, "mismatched size: Uint64");

    static_assert(sizeof(Int8) == 1, "mismatched size: Int8");
    static_assert(sizeof(Int16) == 2, "mismatched size: Int16");
    static_assert(sizeof(Int32) == 4, "mismatched size: Int32");
    static_assert(sizeof(Int64) == 8, "mismatched size: Int64");

    static_assert(sizeof(Byte) == 1, "mismatched size: Byte");
}