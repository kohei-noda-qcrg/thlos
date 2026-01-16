#include "common.h"
#include "kernel.h"

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
