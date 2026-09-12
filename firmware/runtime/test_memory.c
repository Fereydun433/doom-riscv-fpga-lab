#include "memory.h"

int test_memory_functions(void)
{
    unsigned char source[48];
    unsigned char destination[48];

    for (size_t i = 0; i < sizeof(source); i++)
        source[i] = (unsigned char)(i ^ 0xA5u);

    /* Exercise byte offsets, zero length, odd lengths and boundary guards. */
    for (size_t offset = 0; offset < 4; offset++) {
        for (size_t count = 0; count <= 32; count++) {
            for (size_t i = 0; i < sizeof(destination); i++)
                destination[i] = 0xCCu;
            unsigned char *target = destination + 4 + offset;
            if (memset(target, 0x15A, count) != target)
                return 0;
            for (size_t i = 0; i < sizeof(destination); i++) {
                unsigned char expected =
                    i >= 4 + offset && i < 4 + offset + count ? 0x5Au : 0xCCu;
                if (destination[i] != expected)
                    return 0;
            }
            if (memcpy(target, source + offset, count) != target)
                return 0;
            for (size_t i = 0; i < sizeof(destination); i++) {
                unsigned char expected =
                    i >= 4 + offset && i < 4 + offset + count ? source[i - 4] : 0xCCu;
                if (destination[i] != expected)
                    return 0;
            }
            if (memcmp(target, source + offset, count) != 0)
                return 0;
        }
    }

    unsigned char a[3] = {0x80u, 4u, 9u};
    unsigned char b[3] = {0x7Fu, 4u, 9u};
    if (memcmp(a, b, 0) != 0 || memcmp(a, b, 3) <= 0 || memcmp(b, a, 3) >= 0)
        return 0;
    b[0] = a[0];
    b[2] = 10u;
    if (memcmp(a, b, 2) != 0 || memcmp(a, b, 3) >= 0 || memcmp(b, a, 3) <= 0)
        return 0;
    return 1;
}
