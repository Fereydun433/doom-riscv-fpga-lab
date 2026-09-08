static void put_char(char c)
{
    volatile unsigned int *const console =
        (volatile unsigned int *)0x10000000u;

    *console = (unsigned char)c;
}

static void put_text(const char *text)
{
    while (*text != '\0')
    {
        put_char(*text);
        text++;
    }
}

static unsigned int read_timer(void)
{
    const volatile unsigned int *const timer =
        (const volatile unsigned int *)0x10000008u;

    return *timer;
}

int main(void)
{
    put_text("Hello RISC-V\n");

    unsigned int start = read_timer();

    for (unsigned int attempt = 0; attempt < 256; attempt++)
    {
        unsigned int elapsed = read_timer() - start;

        if (elapsed >= 1000u)
        {
            put_text("Timer OK\n");
            return 0;
        }
    }

    put_text("Timer FAIL\n");
    return 1;
}