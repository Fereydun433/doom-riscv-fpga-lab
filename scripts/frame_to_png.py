from pathlib import Path
import struct
import zlib

source = Path("build/riscv/framebuffer.ppm").read_bytes()
header = b"P6\n64 64\n255\n"

if not source.startswith(header):
    raise SystemExit("Unexpected PPM header")

pixels = source[len(header):]

if len(pixels) != 64 * 64 * 3:
    raise SystemExit("Incomplete pixel data")


def chunk(kind, data):
    checksum = zlib.crc32(kind + data) & 0xffffffff

    return (
        struct.pack(">I", len(data))
        + kind
        + data
        + struct.pack(">I", checksum)
    )


rows = b"".join(
    b"\x00" + pixels[y * 192:(y + 1) * 192]
    for y in range(64)
)

png = b"\x89PNG\r\n\x1a\n"
png += chunk(
    b"IHDR",
    struct.pack(">IIBBBBB", 64, 64, 8, 2, 0, 0, 0)
)
png += chunk(b"IDAT", zlib.compress(rows))
png += chunk(b"IEND", b"")

Path("build/riscv/framebuffer.png").write_bytes(png)

print("Saved build/riscv/framebuffer.png (64 x 64)")