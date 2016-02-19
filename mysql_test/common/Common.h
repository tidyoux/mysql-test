//
//  Common.h
//  game3.2alpha0
//
//  Created by haitao on 15-4-14.
//
//

#ifndef game3_2alpha0_Common_h
#define game3_2alpha0_Common_h

#if defined(__osf__)
// Tru64 lacks stdint.h, but has inttypes.h which defines a superset of
// what stdint.h would define.
#include <inttypes.h>
#elif !defined(_MSC_VER)
#include <stdint.h>
#endif

//
// type
//
typedef unsigned int uint;

#ifdef _MSC_VER
typedef __int8  int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
#else
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
#endif

//
// bound
//
#ifdef _MSC_VER
#define MAKE_LONGLONG(x) x##I64
#define MAKE_ULONGLONG(x) x##UI64
#else
#define MAKE_LONGLONG(x) x##LL
#define MAKE_ULONGLONG(x) x##ULL
#endif

namespace IntBound
{
    static const int32 kint32max = 0x7FFFFFFF;
    static const int32 kint32min = -kint32max - 1;
    static const int64 kint64max = MAKE_LONGLONG(0x7FFFFFFFFFFFFFFF);
    static const int64 kint64min = -kint64max - 1;
    static const uint32 kuint32max = 0xFFFFFFFFu;
    static const uint64 kuint64max = MAKE_ULONGLONG(0xFFFFFFFFFFFFFFFF);
}


#endif //game3_2alpha0_Common_h
