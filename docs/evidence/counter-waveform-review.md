# Counter waveform review

## Result

The supplied `tick_counter.vcd` is consistent with the 8-bit counter's specified
behavior at all **262 recorded rising clock edges**. No count changes were
observed away from rising edges after the initial sample.

This review is based on the supplied VCD and the user's passing Verilator run.
It verifies the recorded counter experiment; it does not establish processor
operation, DOOM execution, or physical FPGA timing.

![Observed counter waveforms](counter-waveforms.png)

## Evidence

| Field | Observed value |
| --- | --- |
| Input | `tick_counter.vcd` |
| VCD timescale | 1 ps |
| First / last recorded time | 0 / 2615 ps |
| Recorded timestamps | 524 |
| Rising clock edges checked | 262 |
| Enabled increments, including rollover | 257 |
| Disabled hold edges | 3 |
| Reset sampling edges | 2 |
| State-transition mismatches | 0 |
| Count changes outside rising edges | 0 |

Input SHA-256:

```text
a6ab4f4564bab6ea5dbabc1499cec3af631350fea690c8ab9a6452baeeeb3a76
```

## How the trace was checked

VCD updates were grouped by timestamp, so clock and counter changes recorded
at the same time were evaluated together. At every observed low-to-high clock
transition, the expected output was computed from the previous count and the
recorded control values:

1. If `rst` is high, the expected count is zero.
2. Otherwise, if `enable` is high, the expected count is the previous count
   plus one, modulo 256.
3. Otherwise, the expected count is unchanged.

All 262 observations matched. The file exposes top-level signals and aliases
under the `tick_counter` scope; aliases were counted only once.

## Specific observations

| Recorded time (ps) | Observation |
| --- | --- |
| 5 | Reset is high at the first rising edge; count is zero. |
| 15, 25, 35, 45, 55 | Count advances through 1, 2, 3, 4, 5. |
| 60 | `enable` goes low while count is 5. |
| 65, 75, 85 | Clock rises while counting is disabled; count remains 5. |
| 90 | `enable` goes high again. |
| 95 | The next rising edge advances count from 5 to 6. |
| 2585 | Count reaches 255. |
| 2595 | The next increment rolls over from 255 to 0. |
| 2605 | Count advances from 0 to 1. |
| 2610 | `rst` goes high at a falling clock edge; count remains 1. |
| 2615 | At the rising edge, reset takes priority and count becomes 0. |

The final reset event demonstrates the synchronous reset behavior in this trace:
asserting reset between rising edges does not immediately change the output.
The trace ends at 2615 ps; the final red marker in panel B denotes the last
recorded count value, zero. No trace is extrapolated beyond that sample.

## Time interpretation

The VCD declares `$timescale 1ps`. The testbench advances simulation time by
5 units per half-cycle, giving a 10 ps simulated period. This setting is a
convention of the testbench. It is not a measurement of a physical clock,
maximum achievable FPGA frequency, or the host's execution time.

## Environment reported by the user

| Component | Reported value |
| --- | --- |
| Windows version string | 10.0.26200.9278 |
| Docker Desktop | 4.90.0 |
| Docker Engine | 29.7.2 |
| Docker context | desktop-linux |
| Container OS / architecture | linux/amd64 |
| Base image release | Ubuntu 24.04 |
| Verilator version | Not present in the supplied terminal log |

## Next milestone

The Docker/Verilator counter experiment has passed and its waveform has been
reviewed. The next implementation milestone is a simulated RISC-V CPU with
memory and a console, followed by a small compiled program. DOOM integration
remains planned work.
