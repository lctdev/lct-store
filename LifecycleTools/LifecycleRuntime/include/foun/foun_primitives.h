#ifndef FOUN_PRIMITIVES_H
#define FOUN_PRIMITIVES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef float f32;
typedef double f64;

#if defined(WIN32)
typedef wchar_t c16;
#else
typedef char16_t c16;
#endif

#ifndef NULL
#define NULL 0
#endif

#endif//FOUN_PRIMITIVES_H
