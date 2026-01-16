#!/bin/bash

set -euo pipefail

clang -std=c11 -O2 -g3 -Wall -Wextra \
    --target=riscv32-unknown-elf \
    -fuse-ld=lld \
    -fno-stack-protector \
    -ffreestanding \
    -nostdlib \
    -Wl,-Tkernel.ld -Wl,-Map=kernel.map \
    -o kernel.elf kernel.c

QEMU=${QEMU:-qemu-system-riscv32}

$QEMU -machine virt \
    -bios default \
    -nographic \
    -serial mon:stdio \
    --no-reboot \
    -kernel kernel.elf
