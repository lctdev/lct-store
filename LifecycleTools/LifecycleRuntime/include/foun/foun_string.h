#ifndef FOUN_STRING_H
#define FOUN_STRING_H

#include <stdio.h>

#if defined(WIN32)
#define LCT_SPRINTF(s, n, format, ...)  _snprintf_s(s, n, n, format, __VA_ARGS__)
#define LCT_STRCPY(s, n, src) strcpy_s(s, n, src)
#else
#define LCT_SPRINTF(s, n, format, ...)  snprintf(s, n, format, __VA_ARGS__)
#endif

#endif//FOUN_MATH_H
