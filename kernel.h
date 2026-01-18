#pragma once

#include "common.h"

#define PANIC(fmt, ...)                                                       \
    do {                                                                      \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while(1) {                                                            \
        }                                                                     \
    } while(0)

#define REGISTER(name, val) \
    register long name __asm__(#name) = (val);

struct sbiret {
    long error; // a0
    long value; // a1
};

void putchar(char ch);
