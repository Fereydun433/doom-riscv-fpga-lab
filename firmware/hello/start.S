.section .text.start
.globl _start

_start:
    la sp, _stack_top

    la t0, _bss_start
    la t1, _bss_end

clear_bss:
    bgeu t0, t1, run_main
    sw zero, 0(t0)
    addi t0, t0, 4
    j clear_bss

run_main:
    call main

    li t0, 0x10000004
    sw a0, 0(t0)

halt:
    j halt