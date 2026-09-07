# Development roadmap

Complete each milestone only after saving its evidence. The starter provides
the files for milestone 1; its first actual execution is pending.

| Milestone | Deliverable | Evidence to record | Status |
| --- | --- | --- | --- |
| 0. Publish the project | GitHub repository, README, upstream submodule | Repository URL and initial commit | Pending user setup |
| 1. Establish RTL simulation | Docker build and counter experiment | Tool versions, passing run log, waveform | Files prepared; execution pending |
| 2. Boot a RISC-V system | LiteX CPU, memory, and console | Boot log and a compiled Hello World program | Planned |
| 3. Add platform services | Timer, framebuffer, and keyboard input | Image test, timing checks, input event log | Planned |
| 4. Port DoomGeneric | RISC-V firmware and platform adapter | WAD load and a rendered game frame | Planned |
| 5. Make the game interactive | Reliable input and frame presentation | Playable demo recording and documented limitations | Planned |
| 6. Evaluate the system | Repeatable workload and measurements | Host details, simulation speed, settings, frame metrics | Planned |
| 7. Deploy on a board | Board-specific memory and I/O integration | Build report and physical hardware demo | Optional later work |

## What the DOOM milestone involves

The platform adapter needs initialization, frame presentation, timing, sleep,
and key events. The complete firmware also needs a working C runtime, allocation,
and access to WAD data. Audio can follow a working image-and-input path.

The precise CPU configuration, memory map, framebuffer format, and loading
method will be chosen during the RISC-V milestone. They are not fixed by the
counter exercise.

## Evidence for a portfolio

Keep small run logs, useful screenshots, design decisions, and links to passing
CI runs. Explain each measured result with the project commit and host setup.
Record obstacles and fixes as issues or short development notes.

At the current stage, describe the project as **in progress**. A title such as
"DOOM on a simulated RISC-V system — FPGA learning project" expresses the goal.
Claims about a working port, frame rate, or physical FPGA deployment belong in
the README and CV only after their corresponding milestones are demonstrated.
