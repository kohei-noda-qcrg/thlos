#pragma once
#include <stddef.h>
#include <stdint.h>

typedef uint32_t paddr_t; // phisical addr
typedef uint32_t vaddr_t; // virtual addr

#define va_list  __builtin_va_list
#define va_start __builtin_va_start
#define va_end   __builtin_va_end
#define va_arg   __builtin_va_arg

#define align_up(value, align)   __builtin_align_up(value, align)
#define is_aligned(value, align) __builtin_is_aligned(value, align)

void* memset(void* buf, int c, size_t n);
void* memcpy(void* dst, const void* src, size_t n);
char* strcpy(char* dst, const char* src);
int   strcmp(const char* s1, const char* s2);
void  printf(const char* fmt, ...);
