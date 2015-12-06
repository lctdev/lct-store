#ifndef FOUN_PRIMITIVES_H
#define FOUN_PRIMITIVES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;

typedef float f32;
typedef double f64;

#if defined(LCT_WINDOWS)
typedef wchar_t c16;
#else
typedef char16_t c16;
#endif

#if defined(LCT_IOS)
typedef u64 uptr;
typedef s64 ssiz;
#else
typedef u32 uptr;
typedef s32 ssiz;
#endif

#ifndef NULL
#define NULL 0
#endif

#endif//FOUN_PRIMITIVES_H
