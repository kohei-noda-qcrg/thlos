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

#define READ_CSR(reg)                                         \
    ({                                                        \
        uint64_t __tmp;                                       \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp;                                                \
    })

#define WRITE_CSR(reg, value)                                  \
    ({                                                         \
        uint32_t __tmp = (value);                              \
        __asm__ __volatile__("csrw " #reg ",%0" ::"r"(__tmp)); \
        __tmp;                                                 \
    })

struct sbiret {
    long error; // a0
    long value; // a1
};

struct trap_frame {
    uint32_t ra, gp, tp;
    uint32_t t0, t1, t2, t3, t4, t5, t6;
    uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
    uint32_t s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    uint32_t sp;
} __attribute__((packed));

void putchar(char ch);
