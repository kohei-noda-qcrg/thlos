ENTRY(boot)

SECTIONS{
    . = 0x80200000; /* Base addr */

    /* Code section */
    .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    /* const data section */
    .rodata : ALIGN(4){
        *(.rodata .rodata.*);
    }

    /* writable data section */
    .data : ALIGN(4){
        *(.data .data.*);
    }

    /* writable 0 initialized data section */
    .bss : ALIGN(4){
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    . = ALIGN(4);
    . += 128*1024; /* 128KB */
    __stack_top = .;

    /* allocatable memory section */
    . = ALIGN(4096); /* 4KB alignment/1 page */
    __free_ram = .;
    . += 64*1024*1024; /* 64MB */
    __free_ram_end = .;
}
