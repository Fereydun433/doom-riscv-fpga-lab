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

static unsigned int pixel_color(unsigned int index)
{
    unsigned int x = index & 63u;
    unsigned int y = index >> 6;

    unsigned int red = x << 2;
    unsigned int green = y << 2;
    unsigned int blue = ((x ^ y) & 8u) ? 255u : 32u;

    return (red << 16) | (green << 8) | blue;
}

static int draw_frame(void)
{
    volatile unsigned int *const fb =
        (volatile unsigned int *)0x20000000u;

    for (unsigned int i = 0; i < 4096; i++)
        fb[i] = pixel_color(i);

    for (unsigned int i = 0; i < 4096; i++)
    {
        if (fb[i] != pixel_color(i))
            return 0;
    }

    return 1;
}

extern volatile unsigned int _ram_probe_start[];

static int test_ram_addresses(void)
{
    volatile unsigned int *const probe = _ram_probe_start;

    for (unsigned int pass = 0; pass < 2; pass++) {
        unsigned int seed = pass == 0 ? 0x13579BDFu : 0xECA86420u;

        for (unsigned int i = 0; i < 15; i++)
            probe[i << 14] = seed ^ (i << 16) ^ i;

        for (unsigned int i = 0; i < 15; i++) {
            unsigned int expected = seed ^ (i << 16) ^ i;
            if (probe[i << 14] != expected)
                return 0;
        }
    }

    return 1;
}

int main(void)
{
    put_text("Hello RISC-V\n");

    if (!test_ram_addresses()) {
        put_text("RAM FAIL\n");
        return 3;
    }
    put_text("RAM OK\n");

    unsigned int start = read_timer();

    for (unsigned int attempt = 0; attempt < 256; attempt++)
    {
        unsigned int elapsed = read_timer() - start;

        if (elapsed >= 1000u)
        {
            put_text("Timer OK\n");

            if (!draw_frame())
            {
                put_text("Frame FAIL\n");
                return 2;
            }

            put_text("Frame OK\n");
            return 0;
        }
    }

    put_text("Timer FAIL\n");
    return 1;
}