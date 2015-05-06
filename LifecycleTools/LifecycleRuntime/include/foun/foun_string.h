#ifndef FOUN_STRING_H
#define FOUN_STRING_H

#if defined (WIN32)
#include <stdio.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#if defined(WIN32)
#define LCT_SPRINTF(s, n, format, ...)  _snprintf_s(s, n, n, format, __VA_ARGS__)
#define LCT_STRCPY(s, n, src) strcpy_s(s, n, src)

#define LCT_UNICODE(s) L##s
#else
#define LCT_SPRINTF(s, n, format, ...) snprintf(s, n, format, ##__VA_ARGS__)
#define LCT_STRCPY(s, n, src) strncpy(s, src, n); s[n-1] = '\0'

#define LCT_UNICODE(s) u##s
#endif

#endif//FOUN_MATH_H
