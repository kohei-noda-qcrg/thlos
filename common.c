#include "common.h"
#include "kernel.h"

void* memset(void* buf, int c, size_t n) {
    uint8_t*      p  = (uint8_t*)buf;
    const uint8_t ch = (uint8_t)c;
    while(n--) {
        *p++ = ch;
    }
    return buf;
}

void* memcpy(void* dst, const void* src, size_t n) {
    uint8_t*       d = (uint8_t*)dst;
    const uint8_t* s = (uint8_t*)src;
    while(n--) {
        *d++ = *s++;
    }
    return dst;
}

char* strcpy(char* dst, const char* src) {
    char* d = dst;
    while(*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dst;
}

int strcmp(const char* s1, const char* s2) {
    while(*s1 && *s2) {
        if(*s1 != *s2) {
            break;
        }
        s1++;
        s2++;
    }
    return *(uint8_t*)s1 - *(uint8_t*)s2;
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while(*fmt) {
        switch(*fmt) {
        case '%': // format specifier
            // read next char
            fmt++;
            switch(*fmt) {
            case '\0':
                putchar('%');
                goto end;
            case '%': // literal %
                putchar('%');
                break;
            case 's': { // string
                const char* s = va_arg(args, const char*);
                while(*s) {
                    putchar(*s);
                    s++;
                }
                break;
            }
            case 'd': { // integer in decimal
                int32_t  v         = va_arg(args, int32_t);
                uint32_t magnitude = v;
                if(v < 0) {
                    putchar('-');
                    magnitude = -magnitude;
                }

                uint32_t divisor = 1;
                while(magnitude / divisor > 9) {
                    divisor *= 10;
                }
                while(divisor > 0) {
                    const char ch = '0' + magnitude / divisor;
                    putchar(ch);
                    magnitude %= divisor;
                    divisor /= 10;
                }
                break;
            }
            case 'x': { // integer in hex
                uint32_t v = va_arg(args, uint32_t);
                for(int i = 7; i >= 0; i--) {
                    uint32_t nibble = (v >> (i * 4)) & 0xf;
                    putchar("0123456789abcdef"[nibble]);
                }
                break;
            }
            }
            break;
        default:
            putchar(*fmt);
            break;
        }
        fmt++;
    }
end:
    va_end(args);
}
