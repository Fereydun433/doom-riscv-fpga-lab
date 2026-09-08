OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    ram (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
}

PHDRS
{
    code PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    .text :
    {
        KEEP(*(.text.start))
        *(.text .text.*)
    } > ram :code

    .rodata :
    {
        *(.rodata .rodata.*)
    } > ram :code

    .data :
    {
        *(.data .data.*)
        *(.sdata .sdata.*)
    } > ram :data

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _bss_start = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > ram :data

    _stack_top = ORIGIN(ram) + LENGTH(ram);

    ASSERT(_bss_end <= _stack_top - 4096,
           "Not enough room for stack")
}