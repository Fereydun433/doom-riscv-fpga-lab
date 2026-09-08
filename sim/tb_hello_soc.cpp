#include "Vhello_soc.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    std::ifstream firmware("build/riscv/hello.hex");
    if (!firmware) {
        std::cerr << "FAIL: hello.hex is missing\n";
        return 1;
    }

    VerilatedContext context;
    context.commandArgs(argc, argv);
    context.traceEverOn(true);

    Vhello_soc dut{&context};
    VerilatedVcdC trace;
    dut.trace(&trace, 2);
    trace.open("build/riscv/hello_soc.vcd");

    auto tick = [&]() {
        dut.clk = 0;
        dut.eval();
        trace.dump(context.time());
        context.timeInc(5);

        dut.clk = 1;
        dut.eval();
        trace.dump(context.time());
        context.timeInc(5);
    };

    dut.resetn = 0;
    for (int i = 0; i < 8; ++i)
        tick();
    dut.resetn = 1;

    std::string received;
        const std::string expected = "Hello RISC-V\nTimer OK\n";
    bool passed = false;
    bool stopped = false;

    for (int cycle = 0; cycle < 100000; ++cycle) {
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
            passed = dut.exit_code == 0 && received == expected;

            if (passed)
                                std::cout << "PASS: Hello RISC-V and timer, exit=0.\n";
            else
                std::cerr << "FAIL: wrong output or exit code\n";

            stopped = true;
            break;
        }
    }

    if (!stopped)
        std::cerr << "FAIL: timeout or early finish\n";

    dut.final();
    trace.close();

    return passed ? 0 : 1;
}