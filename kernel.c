typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;
typedef uint32_t      size_t;

extern char __bss, __bss_end, __stack_top; // declare the symbols defined in kernel.ld

void* memset(void* buf, char c, size_t n) {
    uint8_t* p = (uint8_t*)buf;
    while(n--) {
        *p++ = c;
    }
    return buf;
}

void kernel_main(void) {
    memset(&__bss, 0, (size_t)&__bss_end - (size_t)&__bss); // initialize bss section memory with 0
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
