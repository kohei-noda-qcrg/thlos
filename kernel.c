#include "kernel.h"

extern char __bss, __bss_end, __stack_top; // declare the symbols defined in kernel.ld

void* memset(void* buf, char c, size_t n) {
    uint8_t* p = (uint8_t*)buf;
    while(n--) {
        *p++ = c;
    }
    return buf;
}

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid) {
    REGISTER(a0, arg0)
    REGISTER(a1, arg1)
    REGISTER(a2, arg2)
    REGISTER(a3, arg3)
    REGISTER(a4, arg4)
    REGISTER(a5, arg5)
    REGISTER(a6, fid)
    REGISTER(a7, eid)

    __asm__ __volatile__(
        "ecall" // change CPU execution mode fron kernel mode (S-Mode) to OpenSBI mode (M-Mode)
        : "=r"(a0), "=r"(a1)
        : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
        : "memory");

    return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char ch) {
    const int SBI_CONSOLE_PUTCHER_EID = 0x01;
    sbi_call(ch, 0, 0, 0, 0, 0, 0, SBI_CONSOLE_PUTCHER_EID);
}

void kernel_main(void) {
    memset(&__bss, 0, (size_t)&__bss_end - (size_t)&__bss); // initialize bss section memory with 0
    const char* s = "\n\nHello World!\n";
    for(int i = 0; s[i] != '\0'; i++) {
        putchar(s[i]);
    }
    for(;;)
        ;
}

__attribute__((section(".text.boot")))
__attribute__((naked)) // deny to generate the function prologue and epilogue
void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // set __stack_top (end of stack section) addr to sp
        "j kernel_main\n"
        :
        : [stack_top] "r"(&__stack_top));
}
