#ifndef COMMON_H
#define COMMON_H

typedef unsigned long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef long s64;
typedef int s32;
typedef short s16;
typedef char s8;

#define STR_LIT_LEN(string_literal) ((int)(sizeof(string_literal) - 1))

#endif