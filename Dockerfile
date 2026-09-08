FROM ubuntu:24.04

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        git \
        python3 \
        verilator \
 	gcc-riscv64-unknown-elf \
        binutils-riscv64-unknown-elf \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work

COPY Makefile ./
COPY rtl/ rtl/
COPY sim/ sim/

CMD ["make", "sim"]
