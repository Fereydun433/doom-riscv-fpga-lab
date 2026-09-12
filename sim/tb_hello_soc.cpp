#include "Vhello_soc.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    bool ram_test = false;
    std::string firmware_path = "build/riscv/hello.hex";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "+ram-test")
            ram_test = true;
        if (arg.rfind("+firmware=", 0) == 0)
            firmware_path = arg.substr(10);
    }
    std::ifstream firmware(firmware_path);
    if (!firmware) {
        std::cerr << "FAIL: firmware file is missing: " << firmware_path << "\n";
        return 1;
    }

    std::remove("build/riscv/framebuffer.ppm");
    std::remove("build/riscv/framebuffer.png");

    VerilatedContext context;
    context.commandArgs(argc, argv);

    bool tracing = false;
    for (int i = 1; i < argc; i++)
        if (std::string(argv[i]) == "+trace")
            tracing = true;

    context.traceEverOn(tracing);

    Vhello_soc dut{&context};
    VerilatedVcdC trace;

    if (tracing) {
        dut.trace(&trace, 2);
        trace.open("build/riscv/hello_soc.vcd");
    }

    std::array<std::uint32_t, 4096> pixels{};
    std::array<bool, 4096> seen{};

    auto tick = [&]() {
        dut.clk = 0;
        dut.eval();

        if (dut.resetn && dut.mem_valid && dut.mem_ready &&
            !dut.mem_instr && dut.mem_wstrb == 0 &&
            dut.mem_addr >= 0x20000000u &&
            dut.mem_addr < 0x20004000u) {

            unsigned int index =
                (dut.mem_addr - 0x20000000u) >> 2;

            pixels[index] = dut.mem_rdata;
            seen[index] = true;
        }

        if (tracing)
            trace.dump(context.time());

        context.timeInc(5);

        dut.clk = 1;
        dut.eval();

        if (tracing)
            trace.dump(context.time());

        context.timeInc(5);
    };

    dut.resetn = 0;
    for (int i = 0; i < 8; ++i)
        tick();
    dut.resetn = 1;

    std::string received;
    const std::string expected = ram_test
        ? "Hello RISC-V\nRAM OK\nMemory OK\nTimer OK\nFrame OK\n"
        : "Hello RISC-V\nMemory OK\nTimer OK\nFrame OK\n";

    bool passed = false;
    bool stopped = false;

    for (int cycle = 0; cycle < 10000000; ++cycle) {
        if (context.gotFinish())
            break;

        tick();

        if (dut.console_valid) {
            char c = static_cast<char>(dut.console_data);
            received += c;
            std::cout << c << std::flush;
        }

        if (dut.bus_fault) {
            std::cerr << "FAIL: bus fault at 0x"
                      << std::hex << dut.fault_addr << '\n';
            stopped = true;
            break;
        }

        if (dut.trap) {
            std::cerr << "FAIL: CPU trap\n";
            stopped = true;
            break;
        }

        if (dut.done) {
            bool complete = std::all_of(
                seen.begin(), seen.end(),
                [](bool value) { return value; }
            );

            passed = dut.exit_code == 0 &&
                     received == expected &&
                     complete;

            if (passed) {
                std::ofstream image(
                    "build/riscv/framebuffer.ppm",
                    std::ios::binary
                );

                image << "P6\n64 64\n255\n";

                for (std::uint32_t pixel : pixels) {
                    image.put(static_cast<char>(
                        (pixel >> 16) & 255u));
                    image.put(static_cast<char>(
                        (pixel >> 8) & 255u));
                    image.put(static_cast<char>(
                        pixel & 255u));
                }

                image.close();
                passed = static_cast<bool>(image);
            }

            if (passed)
                std::cout
                    << (ram_test
                        ? "PASS: RAM addressing, memory functions, timer and 4096 framebuffer pixels, exit=0.\n"
                        : "PASS: memory functions, timer and 4096 framebuffer pixels, exit=0.\n");
            else
                std::cerr
                    << "FAIL: output, exit code, pixel capture or image file\n";

            stopped = true;
            break;
        }
    }

    if (!stopped)
        std::cerr << "FAIL: timeout or early finish\n";

    dut.final();

    if (tracing)
        trace.close();

    return passed ? 0 : 1;
}
