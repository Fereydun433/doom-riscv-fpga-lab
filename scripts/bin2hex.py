from pathlib import Path
import sys

input_file = Path(sys.argv[1])
output_file = Path(sys.argv[2])

data = input_file.read_bytes()
program_size = len(data)
ram_size = 1024 * 1024

if not 0 < program_size <= ram_size:
    raise SystemExit("Program must fit in 1 MiB RAM")

data += b"\xa5" * (ram_size - program_size)

lines = []

for offset in range(0, ram_size, 4):
    four_bytes = data[offset:offset + 4]
    word = int.from_bytes(four_bytes, "little")
    lines.append(f"{word:08x}")

output_file.write_text(
    "\n".join(lines) + "\n",
    encoding="ascii"
)

print(f"Program size: {program_size} bytes")
print(f"RAM image: {len(lines)} words")