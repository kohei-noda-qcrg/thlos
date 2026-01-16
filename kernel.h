#pragma once

typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;
typedef uint32_t      size_t;

#define REGISTER(name, val) \
    register long name __asm__(#name) = (val);

struct sbiret {
    long error; // a0
    long value; // a1
};
