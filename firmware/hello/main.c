static void put_char(char c)
{
    volatile unsigned int *const console =
        (volatile unsigned int *)0x10000000u;

    *console = (unsigned char)c;
}

int main(void)
{
    const char *text = "Hello RISC-V\n";

    while (*text != '\0')
    {
        put_char(*text);
        text++;
    }

    return 0;
}