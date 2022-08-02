#ifndef VSTD_H
#define VSTD_H


#include <stdlib.h>
#include <stdio.h> 
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>


#ifndef __cplusplus
#error "Must compile as C++"
#endif


#if defined(_MSC_VER)
#define VSTD_CC_MSVC
#elif defined(__clang__)
#define VSTD_CC_CLANG
#elif defined(__GNUC__)
#define VSTD_CC_GCC
#elif defined(__EMSCRIPTEN__)
#error "Emscripten not supported!"
#elif defined(__MINGW32__) || defined(__MINGW64__)
//#error "MinGW not yet supported!"
#else
#error "Unknown compiler!"
#endif


#define INTEGRAL_TYPES(X)      \
    X(u8, unsigned char)       \
    X(u16, unsigned short)     \
    X(u32, unsigned int)       \
    X(u64, unsigned long long) \
    X(s8, signed char)         \
    X(s16, signed short)       \
    X(s32, signed int)         \
    X(s64, signed long long)

#define FLOAT_TYPES(X) \
    X(f32, float)      \
    X(f64, double) 

#define X(name, ctype) using name = ctype;
INTEGRAL_TYPES(X)
FLOAT_TYPES(X)
#undef X


// Don't @ me.
static_assert(sizeof(u8)  == 1);
static_assert(sizeof(s8)  == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(s16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(s32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(s64) == 8);
static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);


#define CAT2(a, b) a##b
#define CAT(a, b) CAT2(a, b)


#define array_length(a) ((sizeof(a))/(sizeof(a[0])))
#define cast(t, v) ((t)(v))
#define unused(x) ((void)x)


#ifndef NULL
#define NULL 0
#endif


#ifndef offsetof
#define offsetof(type, member) ((u64)&(((type*)0)->member))
#endif


template<typename T>
void swap(T *a, T *b) {
	T c = *a;
	*a = *b;
	*b = c;
}

template<typename T>
void swap(T& a, T& b) {
	T c = a;
	a = b;
	b = c;
}


#define BYTE_UNIT_MULTIPLES(X)  \
	X(1,  KILO,   KIBI) 		\
	X(2,  MEGA,   MEBI) 		\
	X(3,  GIGA,   GIBI) 		\
	X(4,  TERA,   TEBI) 		\
	X(5,  PETA,   PEBI) 		\
	X(6,  EXA,    EXI )

#define X(i, dec, bin) 						     \
	constexpr u64 dec##BYTE = pow<u64>(1000, i); \
	constexpr u64 bin##BYTE = pow<u64>(1024, i); \
	constexpr u64 dec##BYTES(u64 n) {		     \
		return n * dec##BYTE;				     \
	}										     \
	constexpr u64 bin##BYTES(u64 n) {		     \
		return n * bin##BYTE;				     \
	}
BYTE_UNIT_MULTIPLES(X)
#undef X


