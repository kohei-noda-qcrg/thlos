#pragma once

#define REGISTER(name, val) \
    register long name __asm__(#name) = (val);

struct sbiret {
    long error; // a0
    long value; // a1
};

void putchar(char ch);
