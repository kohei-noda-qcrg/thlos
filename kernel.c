#include "kernel.h"
#include "common.h"

extern char __bss, __bss_end, __stack_top, __free_ram, __free_ram_end; // declare the symbols defined in kernel.ld

__attribute__((naked))
__attribute__((aligned(4))) void
kernel_entry(void) {
    __asm__ __volatile__(
        "csrw sscratch, sp\n"
        "addi sp, sp, -4 * 31\n"
        ".irp reg,ra,gp,tp,t0,t1,t2,t3,t4,t5,t6,"
        "a0,a1,a2,a3,a4,a5,a6,a7,"
        "s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11\n"
        "sw \\reg, 4*\\@ (sp)\n"
        ".endr\n"

        "csrr a0, sscratch\n"
        "sw a0, 4*30(sp)\n"

        "mv a0, sp\n"
        "call handle_trap\n"

        ".irp reg,ra,gp,tp,t0,t1,t2,t3,t4,t5,t6,"
        "a0,a1,a2,a3,a4,a5,a6,a7,"
        "s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,sp\n"
        "lw \\reg, 4*\\@ (sp)\n"
        ".endr\n"

        "sret\n");
}

void handle_trap(struct trap_frame* f) {
    uint32_t scause  = READ_CSR(scause);
    uint32_t stval   = READ_CSR(stval);
    uint32_t user_pc = READ_CSR(sepc);

    PANIC("unexpected trap scause=%x, stval=%x, spec=%x\n", scause, stval, user_pc);
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

paddr_t alloc_pages(uint32_t npages) {
    static paddr_t next_paddr = (paddr_t)&__free_ram;
    paddr_t        paddr      = next_paddr;
    next_paddr += npages * PAGE_SIZE;

    if(next_paddr > (paddr_t)&__free_ram_end) {
        PANIC("out of memory");
    }

    memset((void*)paddr, 0, npages * PAGE_SIZE);
    return paddr;
}

void kernel_main(void) {
    memset(&__bss, 0, (size_t)&__bss_end - (size_t)&__bss); // initialize bss section memory with 0
    const char* s = "\n\nHello World!\n";
    printf(s);
    printf("1 + 2 %d, %x %%\n", 1 + 2, 0x1234abcd);

    paddr_t paddr0 = alloc_pages(2);
    paddr_t paddr1 = alloc_pages(1);
    printf("alloc_pages test: paddr0=%x\n", paddr0);
    printf("alloc_pages test: paddr1=%x\n", paddr1);

    WRITE_CSR(stvec, (uint32_t)kernel_entry);
    __asm__ __volatile__("unimp"); // invalid instruction
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
