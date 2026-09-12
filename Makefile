.DEFAULT_GOAL := help
.RECIPEPREFIX := >

VERILATOR ?= verilator
RISCV_PREFIX ?= riscv64-unknown-elf-

COUNTER_BIN := build/obj_dir/Vtick_counter
COUNTER_SRC := $(abspath rtl/tick_counter.sv sim/tb_tick_counter.cpp)

CPU_BIN := build/riscv/obj_dir/Vhello_soc
CPU_SRC := $(abspath rtl/hello_soc.sv third_party/picorv32/picorv32.v sim/tb_hello_soc.cpp)

FW := build/riscv/hello
RUNTIME_SRC := firmware/runtime/memory.c firmware/runtime/test_memory.c
FW_FLAGS := -march=rv32i -mabi=ilp32 -mno-relax -msmall-data-limit=0
FW_FLAGS += -ffreestanding -fno-builtin -Wall -Wextra -O0 -g

.PHONY: help doctor sim sim-riscv sim-ram
.DELETE_ON_ERROR:

help:
>@echo "make sim: run the counter"
>@echo "make sim-riscv: build and run Hello RISC-V"
>@echo "make sim-ram: run the reserved RAM address test and integration checks"
>@echo "make doctor: show tool versions"

doctor:
>$(VERILATOR) --version
>$(RISCV_PREFIX)gcc --version
>g++ --version
>python3 --version
>git --version

$(COUNTER_BIN): $(COUNTER_SRC) Makefile
>mkdir -p build
>$(VERILATOR) -Wall --cc --exe --build --trace --top-module tick_counter --Mdir build/obj_dir -CFLAGS "-std=c++17" $(COUNTER_SRC)

sim: $(COUNTER_BIN)
>./$(COUNTER_BIN)

$(FW).elf: firmware/hello/main.c firmware/hello/start.S firmware/hello/link.ld $(RUNTIME_SRC) firmware/runtime/memory.h Makefile
>mkdir -p build/riscv
>$(RISCV_PREFIX)gcc $(FW_FLAGS) -nostdlib -Wl,--build-id=none -T firmware/hello/link.ld firmware/hello/start.S firmware/hello/main.c $(RUNTIME_SRC) -o $@

$(FW).bin: $(FW).elf
>$(RISCV_PREFIX)objcopy -O binary $< $@

$(FW).hex: $(FW).bin scripts/bin2hex.py
>python3 scripts/bin2hex.py $< $@

$(CPU_BIN): $(CPU_SRC) sim/picorv32.vlt Makefile
>mkdir -p build/riscv
>$(VERILATOR) -Wall --cc --exe --build --trace --timescale-override 1ns/1ns --top-module hello_soc --Mdir build/riscv/obj_dir -CFLAGS "-std=c++17" $(abspath sim/picorv32.vlt) $(CPU_SRC)

sim-riscv: $(FW).hex $(CPU_BIN)
>./$(CPU_BIN)

RAM_FW := build/riscv/ram_test

$(RAM_FW).elf: firmware/hello/main.c firmware/hello/start.S firmware/hello/link_ram_test.ld $(RUNTIME_SRC) firmware/runtime/memory.h Makefile
>mkdir -p build/riscv
>$(RISCV_PREFIX)gcc $(FW_FLAGS) -DRAM_ADDRESS_TEST -nostdlib -Wl,--build-id=none -T firmware/hello/link_ram_test.ld firmware/hello/start.S firmware/hello/main.c $(RUNTIME_SRC) -o $@

$(RAM_FW).bin: $(RAM_FW).elf
>$(RISCV_PREFIX)objcopy -O binary $< $@

$(RAM_FW).hex: $(RAM_FW).bin scripts/bin2hex.py
>python3 scripts/bin2hex.py $< $@

sim-ram: $(RAM_FW).hex $(CPU_BIN)
>./$(CPU_BIN) +ram-test +firmware=$(RAM_FW).hex
