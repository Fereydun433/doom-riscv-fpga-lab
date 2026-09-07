# SPDX-License-Identifier: GPL-2.0-only
.DEFAULT_GOAL := help

VERILATOR ?= verilator
OBJ_DIR := build/obj_dir
SIM_BIN := $(OBJ_DIR)/Vtick_counter
RTL := $(abspath rtl/tick_counter.sv)
TB := $(abspath sim/tb_tick_counter.cpp)

.PHONY: help doctor sim

help:
	@printf '%s\n' 'make doctor: report installed tool versions' 'make sim: build and run the counter experiment'

doctor:
	$(VERILATOR) --version
	g++ --version
	python3 --version
	git --version

$(SIM_BIN): $(RTL) $(TB) Makefile
	mkdir -p build
	$(VERILATOR) -Wall --cc --exe --build --trace \
		--top-module tick_counter --Mdir $(OBJ_DIR) \
		-CFLAGS "-std=c++17" $(RTL) $(TB)

sim: $(SIM_BIN)
	./$(SIM_BIN)
