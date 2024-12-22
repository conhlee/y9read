#include "common.h"

#include <stdio.h>

#include <stdlib.h>

#include <stdarg.h>

#include <string.h>

void panic(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    printf("\nPANIC: %s\n\n", buffer);

    va_end(args);

    exit(1);

    __builtin_unreachable();
}

void warn(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    printf("\nWARN: %s\n\n", buffer);

    va_end(args);
}

float byteswapFloat(float value) {
    u32 temp = __builtin_bswap32(*(u32*)&value);
    return *(float*)&temp;
}