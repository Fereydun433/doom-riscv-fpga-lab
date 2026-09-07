// SPDX-License-Identifier: GPL-2.0-only
#include "Vtick_counter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

int main(int argc, char** argv) {
    auto context = std::make_unique<VerilatedContext>();
    context->commandArgs(argc, argv);
    context->traceEverOn(true);
    Vtick_counter dut{context.get()};
    VerilatedVcdC trace;
    dut.trace(&trace, 5);
    trace.open("build/tick_counter.vcd");

    dut.clk = 0;
    dut.rst = 1;
    dut.enable = 1;

    auto tick = [&]() {
        dut.clk = 0;
        dut.eval();
        trace.dump(context->time());
        context->timeInc(5);
        dut.clk = 1;
        dut.eval();
        trace.dump(context->time());
        context->timeInc(5);
    };

    auto expect = [&](unsigned value, const char* phase) {
        if (static_cast<unsigned>(dut.count) != value) {
            throw std::runtime_error(std::string(phase) + ": expected " +
                std::to_string(value) + ", observed " +
                std::to_string(static_cast<unsigned>(dut.count)));
        }
    };

    int result = 0;
    try {
        // Reset takes priority even when counting is enabled.
        tick();
        expect(0, "initial reset");

        dut.rst = 0;
        for (unsigned value = 1; value <= 5; ++value) {
            tick();
            expect(value, "increment");
        }

        dut.enable = 0;
        for (unsigned cycle = 0; cycle < 3; ++cycle) {
            tick();
            expect(5, "disabled hold");
        }

        dut.enable = 1;
        for (unsigned value = 6; value <= 255; ++value) {
            tick();
            expect(value, "count to maximum");
        }
        tick();
        expect(0, "rollover");
        tick();
        expect(1, "count after rollover");

        dut.rst = 1;
        tick();
        expect(0, "reset after counting");
        std::cout << "PASS: reset, increment, hold, rollover.\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        result = 1;
    }

    dut.final();
    trace.close();
    return result;
}
